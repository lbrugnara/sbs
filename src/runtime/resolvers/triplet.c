#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include <fllib/containers/List.h>
#include "triplet.h"
#include "environment.h"
#include "toolchain.h"
#include "configuration.h"
#include "../../utils.h"

FlList* sbs_triplet_resolve_all(SbsContext *context, const char *env, const char *toolchain, const char *config, bool script_mode)
{
    FlList *triplets = fl_list_new();

    for (size_t i=0; i < fl_array_length(context->preset->envs); i++)
    {
        // Start with environments
        const char *env_name = context->preset->envs[i];

        if (env != NULL && !flm_cstring_equals(env, env_name))
            continue;

        SbsContext *triplet_ctx = sbs_context_copy(context);
        triplet_ctx->env = sbs_env_resolve(triplet_ctx, env_name); // get the env

        if (!triplet_ctx->env)
            goto env_clean_ctx;

        // Make sure the environment can run on the host
        if (!script_mode && (triplet_ctx->host->os != triplet_ctx->env->os || !fl_array_contains(triplet_ctx->env->arch, &triplet_ctx->host->arch)))
            goto env_clean_ctx;

        // At this point it is safe to create a triplet
        SbsTriplet *triplet = sbs_triplet_new(triplet_ctx);

        fl_hashtable_add(triplet->context->symbols->variables, "sbs.env", triplet->context->env->name);

        fl_list_append(triplets, triplet);

        continue;

        env_clean_ctx: sbs_context_free(triplet_ctx);
    }

    struct FlListNode *node = fl_list_head(triplets);
    while (node)
    {
        SbsTriplet *triplet = (SbsTriplet*) node->value;

        for (size_t i = 0; i < fl_array_length(context->preset->toolchains); i++)
        {
            char *toolchain_name = context->preset->toolchains[i];

            if (toolchain != NULL && !flm_cstring_equals(toolchain, toolchain_name))
                continue;
            
            SbsContext *triplet_ctx = sbs_context_copy(triplet->context);
            triplet_ctx->toolchain = sbs_toolchain_resolve(triplet_ctx, toolchain_name); // get the toolchain
            
            if (!triplet_ctx->toolchain)
                goto tc_clean_ctx;

            // At this point it is safe to create a triplet
            SbsTriplet *new_triplet = sbs_triplet_new(triplet_ctx);

            fl_hashtable_set(new_triplet->context->symbols->variables, "sbs.env", new_triplet->context->env->name);

            fl_hashtable_add(new_triplet->context->symbols->variables, "sbs.toolchain", new_triplet->context->toolchain->name);

            if (new_triplet->context->toolchain->compiler.bin != NULL)
                fl_hashtable_add(new_triplet->context->symbols->variables, "sbs.compiler", new_triplet->context->toolchain->compiler.bin);

            if (new_triplet->context->toolchain->archiver.bin != NULL)
                fl_hashtable_add(new_triplet->context->symbols->variables, "sbs.archiver", new_triplet->context->toolchain->archiver.bin);

            if (new_triplet->context->toolchain->linker.bin != NULL)
                fl_hashtable_add(new_triplet->context->symbols->variables, "sbs.linker", new_triplet->context->toolchain->linker.bin);

            fl_list_insert_before(triplets, node, new_triplet);

            continue;

            tc_clean_ctx: sbs_context_free(triplet_ctx);
        }

        struct FlListNode *to_rem = node;
        node = node->next;

        sbs_triplet_free((SbsTriplet*) to_rem->value);
        fl_list_remove(triplets, to_rem);
    }

    node = fl_list_head(triplets);
    while (node)
    {
        SbsTriplet *triplet = (SbsTriplet*) node->value;

        for (size_t i = 0; i < fl_array_length(context->preset->configs); i++)
        {
            char *config_name = context->preset->configs[i];

            if (config != NULL && !flm_cstring_equals(config, config_name))
                continue;
            
            SbsContext *triplet_ctx = sbs_context_copy(triplet->context);
            triplet_ctx->config = sbs_config_resolve(triplet_ctx, config_name); // get the config
            
            if (!triplet_ctx->config)
                goto config_clean_ctx;

            // At this point it is safe to create a triplet
            SbsTriplet *new_triplet = sbs_triplet_new(triplet_ctx);

            fl_hashtable_set(new_triplet->context->symbols->variables, "sbs.env", new_triplet->context->env->name);
            fl_hashtable_set(new_triplet->context->symbols->variables, "sbs.toolchain", new_triplet->context->toolchain->name);
            fl_hashtable_add(new_triplet->context->symbols->variables, "sbs.config", new_triplet->context->config->name);

            fl_list_insert_before(triplets, node, new_triplet);

            continue;

            config_clean_ctx: sbs_context_free(triplet_ctx);
        }

        struct FlListNode *to_rem = node;
        node = node->next;

        sbs_triplet_free((SbsTriplet*) to_rem->value);
        fl_list_remove(triplets, to_rem);
    }

    return triplets;
}
