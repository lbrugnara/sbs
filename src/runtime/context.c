#include <fllib/Mem.h>
#include "context.h"

#include "../result.h"
#include "../lang/file.h"

#include "executor.h"
#include "environment.h"
#include "configuration.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"

static inline bool resolve_executor(SbsContext *context, bool script_mode, SbsResult *result)
{
    context->executor = sbs_executor_new(context->env->type, context->env->args, context->env->variables, context->env->terminal);

    if (context->executor == NULL)
    {
        *result = sbs_result_print_reason(SBS_RES_ERROR);
        return false;
    }

    if (script_mode)
        sbs_executor_enable_script_mode(context->executor);

    return true;
}

static inline bool resolve_target(SbsContext *context, const char *targetarg, SbsResult *result)
{
    const char *target_name = context->preset && context->preset->target ? context->preset->target : NULL;

    if (targetarg)
        target_name = targetarg;

    if (!target_name)
    {
        *result = sbs_result_print_reason(SBS_RES_MISSING_TARGET_ARG);
        return false;
    }

    // Resolve target
    context->target = sbs_target_resolve(context, target_name, NULL);
    if (context->target == NULL)
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);
        return false;
    }

    fl_hashtable_add(context->symbols->variables, "$sbs.target", context->target->name);

    return true;
}

static inline bool resolve_config(SbsContext *context, const char *configarg, SbsResult *result)
{
    const char *configuration_name = context->preset && context->preset->config ? context->preset->config : NULL;

    if (configarg)
        configuration_name = configarg;

    if (!configuration_name)
    {
        *result = sbs_result_print_reason(SBS_RES_MISSING_CONFIG_ARG);
        return false;
    }

    if (!fl_hashtable_has_key(context->file->configurations, configuration_name))
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);
        return false;
    }

    // Resolve configuration
    context->config = sbs_config_resolve(context, configuration_name);
    if (context->config == NULL)
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);
        return false;
    }

    fl_hashtable_add(context->symbols->variables, "$sbs.config", context->config->name);

    return true;
}

static inline bool resolve_toolchain(SbsContext *context, const char *toolchainarg, SbsResult *result)
{
    const char *toolchain_name = context->preset && context->preset->toolchain ? context->preset->toolchain : NULL;

    if (toolchainarg)
        toolchain_name = toolchainarg;

    if (!toolchain_name)
    {
        *result = sbs_result_print_reason(SBS_RES_MISSING_TOOLCHAIN_ARG);
        return false;
    }

    // Check if the resources exist
    if (!fl_hashtable_has_key(context->file->toolchains, toolchain_name))
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
        return false;
    }

    // Resolve toolchain
    context->toolchain = sbs_toolchain_resolve(context, toolchain_name);
    if (context->toolchain == NULL)
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
        return false;
    }

    fl_hashtable_add(context->symbols->variables, "$sbs.toolchain", context->toolchain->name);

    return true;
}

static inline bool resolve_env(SbsContext *context, const char *envarg, SbsResult *result)
{
    const char *env_name = NULL;

    if (envarg == NULL)
    {
        // If the user did not provide an environment name, the "env" property within the preset must be
        // valid and it must contain at least 1 environment
        if (context->preset == NULL || context->preset->env == NULL || fl_array_length(context->preset->env) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_ENV_ARG);
            return false;
        }

        for (size_t i=0; i < fl_array_length(context->preset->env); i++)
        {
            SbsEnv *tmp = sbs_env_resolve(context, context->preset->env[i]);

            // TODO: Error handling here or previous validation?
            if (!tmp) continue;

            // Make sure the environment can run on the host
            if (context->host->os != tmp->os || !fl_array_contains(tmp->arch, &context->host->arch))
                continue;

            // We've found it
            context->env = tmp;
            break;
        }

        if (context->env == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_MISMATCH_ENV);
            return false;
        }
    }
    else
    {
        // If the user requested a particular environment we try to resolve that
        // particular environment. 
        // TODO: We should check here that if the env can't run on the host, the build
        // command must contain an argument like "--script" or "--dry-run" or something like
        // that that allows creating an script with all the steps without actually executing it
        context->env = sbs_env_resolve(context, envarg);
        
        if (context->env == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_INVALID_ENV, env_name);
            return false;
        }
    }

    // TODO: Allow override of the architecture using arguments
    context->env->host = sbs_host_new(context->env->os, sbs_host_arch());

    fl_hashtable_add(context->symbols->variables, "$sbs.env", context->env->name);

    return true;
}

static inline bool resolve_preset(SbsContext *context, const char *preset_name, SbsResult *result)
{
    context->preset = sbs_preset_resolve(context, preset_name);

    if (context->preset == NULL)
    {
        *result = sbs_result_print_reason(SBS_RES_INVALID_PRESET, preset_name);
        return false;
    }

    return true;
}

static inline bool resolve_host(SbsContext *context, SbsResult *result)
{
    context->host = sbs_host_get_info();

    if (context->host->os == SBS_OS_UNK)
    {
        *result = sbs_result_print_reason(SBS_RES_UNK_OS);
        return false;
    }

    if (context->host->arch == SBS_ARCH_UNK)
    {
        *result = sbs_result_print_reason(SBS_RES_UNK_ARCH);
        return false;
    }

    fl_hashtable_add(context->symbols->variables, "$sbs.os", sbs_host_os_to_str(context->host->os));
    fl_hashtable_add(context->symbols->variables, "$sbs.arch", sbs_host_arch_to_str(context->host->arch));

    return true;
}

SbsContext* sbs_context_new(const SbsFile *file, SbsContextArgs *args, SbsResult *result)
{
    SbsContext *ctx = fl_malloc(sizeof(SbsContext));

    ctx->file = file;
    ctx->symbols = sbs_eval_context_new();

    // If the host information is not complete, we can't continue
    if (!resolve_host(ctx, result))
        goto init_error;

    // Check if the build uses a preset and in that case make sure it is a valid preset
    if (args->preset && !resolve_preset(ctx, args->preset, result))
        goto init_error;

    // From now on all the resources can evaluate the for clauses
    if (!resolve_env(ctx, args->env, result))
        goto init_error;

    if (!resolve_toolchain(ctx, args->toolchain, result))
        goto init_error;

    if (!resolve_config(ctx, args->config, result))
        goto init_error;
    
    if (!resolve_target(ctx, args->target, result))
        goto init_error;

    if (!resolve_executor(ctx, args->script_mode, result))
        goto init_error;

    return ctx;

init_error:
    sbs_context_free(ctx);

    return NULL;
}

void sbs_context_free(SbsContext *context)
{
    if (context->config) sbs_config_free(context->config);
    if (context->target) sbs_target_free(context->target);
    if (context->toolchain) sbs_toolchain_free(context->toolchain);
    if (context->env) sbs_env_free(context->env);
    if (context->preset) sbs_preset_free(context->preset);
    if (context->host) sbs_host_free(context->host);
    if (context->symbols) sbs_eval_context_free(context->symbols);
    if (context->executor) sbs_executor_free(context->executor);

    fl_free(context);
}

SbsContext* sbs_context_copy(const SbsContext *ctx)
{
    SbsResult result;

    SbsContext *copy = sbs_context_new(ctx->file, &(SbsContextArgs) {
        .preset = ctx->preset != NULL ? ctx->preset->name : NULL,
        .env = ctx->env != NULL ? ctx->env->name : NULL,
        .toolchain = ctx->toolchain != NULL ? ctx->toolchain->name : NULL,
        .config = ctx->config != NULL ? ctx->config->name : NULL,
        .target = ctx->target != NULL ? ctx->target->name : NULL,
        .script_mode = (ctx->executor != NULL ? sbs_executor_is_script_mode(ctx->executor) : false),
    }, &result);

    return copy;
}
