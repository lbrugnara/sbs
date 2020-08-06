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
    context->executor = sbs_executor_new(context->env);

    if (context->executor == NULL)
        return false;

    if (script_mode)
        sbs_executor_enable_script_mode(context->executor);

    return true;
}

static inline bool resolve_preset(SbsContext *context, const char *preset_name)
{
    context->preset = sbs_preset_resolve(context->resolvectx, preset_name);
    return context->preset != NULL;
}

SbsTriplet** sbs_triplet_find(const SbsFile *file, const char *preset, const char *env, const char *arch, const char *toolchain, const char *config, bool script_mode)
{
    // Our return value
    SbsTriplet **triplets_array = NULL;

    // Create a context object to resolve the triplets
    SbsContext *context = sbs_context_new(file);

    if (context == NULL)
        return NULL;

    // TODO: check if this is ok here
    context->resolvectx->script_mode = script_mode;

    // Check if the build uses a preset and in that case make sure it is a valid preset
    if (preset != NULL && !resolve_preset(context, preset))
        goto preset_error;

    triplets_array = fl_array_new(sizeof(SbsTriplet*), 0);
    FlList *triplets_list = sbs_triplet_resolve_all(context, env, arch, toolchain, config, script_mode);

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

FlList* sbs_triplet_resolve_all(SbsContext *context, const char *env, const char *arch, const char *toolchain, const char *config, bool script_mode)
{
    FlList *triplets = fl_list_new();

    if (context->preset == NULL)
    {
        SbsContext *tripletctx = sbs_context_copy(context);
        tripletctx->env = sbs_env_resolve(tripletctx->resolvectx, env, arch);
        tripletctx->toolchain = sbs_toolchain_resolve(tripletctx->resolvectx, toolchain);
        tripletctx->config = sbs_config_resolve(tripletctx->resolvectx, config);
        fl_list_append(triplets, sbs_triplet_new(tripletctx));
        return triplets;
    }

    const char **environments = env != NULL ? (const char *[]){ env } : (const char **) context->preset->envs;
    size_t environments_count = env != NULL ? 1 : fl_array_length(context->preset->envs);
    for (size_t i=0; i < environments_count; i++)
    {
        // Start with environments
        const char *env_name = environments[i];

        SbsContext *tripletctx = sbs_context_copy(context);
        tripletctx->env = sbs_env_resolve(tripletctx->resolvectx, env_name, arch); // get the env

        if (!tripletctx->env)
            goto env_clean_ctx;

        // At this point it is safe to create a triplet
        fl_list_append(triplets, sbs_triplet_new(tripletctx));

        continue;

        env_clean_ctx: sbs_context_free(tripletctx);
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
            
            SbsContext *tripletctx = sbs_context_copy(triplet->context);
            tripletctx->toolchain = sbs_toolchain_resolve(tripletctx->resolvectx, toolchain_name); // get the toolchain
            
            if (!tripletctx->toolchain)
                goto tc_clean_ctx;

            // At this point it is safe to create a triplet
            fl_list_insert_before(triplets, node, sbs_triplet_new(tripletctx));

            continue;

            tc_clean_ctx: sbs_context_free(tripletctx);
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
            
            SbsContext *tripletctx = sbs_context_copy(triplet->context);
            tripletctx->config = sbs_config_resolve(tripletctx->resolvectx, config_name); // get the config
            
            if (!tripletctx->config)
                goto config_clean_ctx;

            // At this point it is safe to create a triplet
            fl_list_insert_before(triplets, node, sbs_triplet_new(tripletctx));

            continue;

            config_clean_ctx: sbs_context_free(tripletctx);
        }

        struct FlListNode *to_rem = node;
        node = node->next;

        sbs_triplet_free((SbsTriplet*) to_rem->value);
        fl_list_remove(triplets, to_rem);
    }

    return triplets;
}
