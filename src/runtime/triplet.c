#include <fllib/Mem.h>
#include "triplet.h"
#include "environment.h"
#include "toolchain.h"
#include "configuration.h"
#include "preset.h"
#include "executor.h"
#include "../utils.h"

SbsTriplet* sbs_triplet_new(SbsContext *context)
{
    SbsTriplet *triplet = fl_malloc(sizeof(SbsTriplet));

    triplet->context = context;

    return triplet;
}

void sbs_triplet_free(SbsTriplet *triplet)
{
    if (triplet->context)
        sbs_context_free(triplet->context);

    fl_free(triplet);
}

static inline bool resolve_executor(SbsContext *context, bool script_mode)
{
    context->executor = sbs_executor_new(context->env->type, context->env->args, context->env->variables, context->env->terminal);

    if (context->executor == NULL)
        return false;

    if (script_mode)
        sbs_executor_enable_script_mode(context->executor);

    return true;
}

static inline bool resolve_preset(SbsContext *context, const char *preset_name)
{
    context->preset = sbs_preset_resolve(context->resolvectx, preset_name);

    if (context->preset != NULL)
        fl_hashtable_add(context->evalctx->variables, "sbs.preset", preset_name);

    return context->preset != NULL;
}

SbsTriplet** sbs_triplet_find(const SbsFile *file, const char *preset, const char *env, const char *toolchain, const char *config, bool script_mode)
{
    // Our return value
    SbsTriplet **triplets_array = NULL;

    // Create a context object to resolve the triplets
    SbsContext *context = sbs_context_new(file);

    if (context == NULL)
        return NULL;

    // Check if the build uses a preset and in that case make sure it is a valid preset
    if (preset != NULL && !resolve_preset(context, preset))
        goto preset_error;

    triplets_array = fl_array_new(sizeof(SbsTriplet*), 0);
    FlList *triplets_list = sbs_triplet_resolve_all(context, env, toolchain, config, script_mode);

    struct FlListNode *node = fl_list_head(triplets_list);
    while (node)
    {
        SbsTriplet *triplet = (SbsTriplet*) node->value;

        if (!resolve_executor(triplet->context, script_mode))
        {
            sbs_triplet_free(triplet);
            goto next_node;
        }
        
        triplets_array = fl_array_append(triplets_array, &triplet);

next_node:
        node = node->next; // Update for next iteration
    }

    fl_list_free(triplets_list);

preset_error:
    if (context)
        sbs_context_free(context);

    return triplets_array;
}

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
        triplet_ctx->env = sbs_env_resolve(triplet_ctx->resolvectx, env_name); // get the env

        if (!triplet_ctx->env)
            goto env_clean_ctx;

        // Make sure the environment can run on the host
        if (!script_mode && (triplet_ctx->host->os != triplet_ctx->env->os || !fl_array_contains(triplet_ctx->env->arch, &triplet_ctx->host->arch)))
            goto env_clean_ctx;

        // At this point it is safe to create a triplet
        SbsTriplet *triplet = sbs_triplet_new(triplet_ctx);

        fl_hashtable_add(triplet->context->evalctx->variables, "sbs.env", triplet->context->env->name);

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
            triplet_ctx->toolchain = sbs_toolchain_resolve(triplet_ctx->resolvectx, toolchain_name); // get the toolchain
            
            if (!triplet_ctx->toolchain)
                goto tc_clean_ctx;

            // At this point it is safe to create a triplet
            SbsTriplet *new_triplet = sbs_triplet_new(triplet_ctx);

            fl_hashtable_set(new_triplet->context->evalctx->variables, "sbs.env", new_triplet->context->env->name);

            fl_hashtable_add(new_triplet->context->evalctx->variables, "sbs.toolchain", new_triplet->context->toolchain->name);

            if (new_triplet->context->toolchain->compiler.bin != NULL)
                fl_hashtable_add(new_triplet->context->evalctx->variables, "sbs.compiler", new_triplet->context->toolchain->compiler.bin);

            if (new_triplet->context->toolchain->archiver.bin != NULL)
                fl_hashtable_add(new_triplet->context->evalctx->variables, "sbs.archiver", new_triplet->context->toolchain->archiver.bin);

            if (new_triplet->context->toolchain->linker.bin != NULL)
                fl_hashtable_add(new_triplet->context->evalctx->variables, "sbs.linker", new_triplet->context->toolchain->linker.bin);

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
            triplet_ctx->config = sbs_config_resolve(triplet_ctx->resolvectx, config_name); // get the config
            
            if (!triplet_ctx->config)
                goto config_clean_ctx;

            // At this point it is safe to create a triplet
            SbsTriplet *new_triplet = sbs_triplet_new(triplet_ctx);

            fl_hashtable_set(new_triplet->context->evalctx->variables, "sbs.env", new_triplet->context->env->name);
            fl_hashtable_set(new_triplet->context->evalctx->variables, "sbs.toolchain", new_triplet->context->toolchain->name);
            fl_hashtable_add(new_triplet->context->evalctx->variables, "sbs.config", new_triplet->context->config->name);

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
