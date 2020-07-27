#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/containers/Hashtable.h>
#include "context.h"
#include "executor.h"
#include "resolvers/environment.h"
#include "resolvers/configuration.h"
#include "resolvers/preset.h"
#include "resolvers/target.h"
#include "resolvers/toolchain.h"
#include "../result.h"
#include "../lang/file.h"

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
    if (targetarg == NULL)
    {
        // If the user did not provide a target name, the "targets" property within the preset must be
        // valid and it must contain at least 1 target
        if (context->preset == NULL || context->preset->targets == NULL || fl_array_length(context->preset->targets) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_TARGET_ARG);
            return false;
        }

        context->targets = fl_array_new(sizeof(SbsTarget*), fl_array_length(context->preset->targets));
        for (size_t i = 0; i < fl_array_length(context->preset->targets); i++)
        {
            char *target_name = context->preset->targets[i];

            // Resolve target
            SbsTarget *tmp = sbs_target_resolve(context, target_name, NULL);
            if (tmp == NULL)
            {
                *result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);
                return false;
            }   

            context->targets[i] = tmp;
        }
    }
    else
    {
        SbsTarget *tmp = sbs_target_resolve(context, targetarg, NULL);
        if (tmp == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, targetarg);
            return false;
        }

        context->targets = fl_array_new(sizeof(SbsTarget*), 1);
        context->targets[0] = tmp;
        fl_hashtable_add(context->symbols->variables, "sbs.target", targetarg);
    }

    return true;
}

static inline bool resolve_config(SbsContext *context, const char *configarg, SbsResult *result)
{
    if (configarg == NULL)
    {
        // If the user did not provide a config name, the "configs" property within the preset must be
        // valid and it must contain at least 1 config
        if (context->preset == NULL || context->preset->configs == NULL || fl_array_length(context->preset->configs) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_CONFIG_ARG);
            return false;
        }

        for (size_t i = 0; i < fl_array_length(context->preset->configs); i++)
        {
            char *config_name = context->preset->configs[i];

            SbsConfiguration *tmp = sbs_config_resolve(context, config_name);
            if (tmp == NULL)
            {
                *result = sbs_result_print_reason(SBS_RES_INVALID_CONFIG, config_name);
                return false;
            }

            context->config = tmp;
            break;
        }

        if (context->config == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_MISMATCH_CONFIG);
            return false;
        }
    }
    else
    {
        // Resolve configuration
        context->config = sbs_config_resolve(context, configarg);
        if (context->config == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configarg);
            return false;
        }
    }

    fl_hashtable_add(context->symbols->variables, "sbs.config", context->config->name);

    return true;
}

static inline bool resolve_toolchain(SbsContext *context, const char *toolchainarg, SbsResult *result)
{
    if (toolchainarg == NULL)
    {
        // If the user did not provide a toolchain name, the "toolchains" property within the preset must be
        // valid and it must contain at least 1 toolchain
        if (context->preset == NULL || context->preset->toolchains == NULL || fl_array_length(context->preset->toolchains) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_TOOLCHAIN_ARG);
            return false;
        }

        for (size_t i = 0; i < fl_array_length(context->preset->toolchains); i++)
        {
            const char *toolchain_name = context->preset->toolchains[i];

            // Resolve toolchain
            SbsToolchain *tmp = sbs_toolchain_resolve(context, toolchain_name);
            if (tmp == NULL)
            {
                *result = sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
                return false;
            }

            context->toolchain = tmp;
            break;
        }

        if (context->toolchain == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_MISMATCH_TOOLCHAIN);
            return false;
        }
    }
    else
    {
        // Resolve toolchain
        context->toolchain = sbs_toolchain_resolve(context, toolchainarg);

        if (context->toolchain == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchainarg);
            return false;
        }
    }    

    fl_hashtable_add(context->symbols->variables, "sbs.toolchain", context->toolchain->name);

    return true;
}

static inline bool resolve_env(SbsContext *context, const char *envarg, SbsResult *result)
{
    if (envarg == NULL)
    {
        // If the user did not provide an environment name, the "envs" property within the preset must be
        // valid and it must contain at least 1 environment
        if (context->preset == NULL || context->preset->envs == NULL || fl_array_length(context->preset->envs) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_ENV_ARG);
            return false;
        }

        for (size_t i=0; i < fl_array_length(context->preset->envs); i++)
        {
            SbsEnv *tmp = sbs_env_resolve(context, context->preset->envs[i]);

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
            *result = sbs_result_print_reason(SBS_RES_INVALID_ENV, envarg);
            return false;
        }
    }

    // TODO: Allow override of the architecture using arguments
    context->env->host = sbs_host_new(context->env->os, sbs_host_arch());

    fl_hashtable_add(context->symbols->variables, "sbs.env", context->env->name);

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

    fl_hashtable_add(context->symbols->variables, "sbs.os", sbs_host_os_to_str(context->host->os));
    fl_hashtable_add(context->symbols->variables, "sbs.arch", sbs_host_arch_to_str(context->host->arch));

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
    if (context->targets) fl_array_free_each_pointer(context->targets, (FlArrayFreeElementFunc) sbs_target_free);
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
        .target = ctx->targets != NULL && fl_array_length(ctx->targets) == 1 ? ctx->targets[0]->name : NULL,
        .script_mode = (ctx->executor != NULL ? sbs_executor_is_script_mode(ctx->executor) : false),
    }, &result);

    return copy;
}

SbsContext* sbs_context_copy_args(const SbsContext *ctx, SbsContextArgs *args)
{
    SbsResult result;

    SbsContext *copy = sbs_context_new(ctx->file, &(SbsContextArgs) {
        .preset = args->preset ? args->preset : ( ctx->preset != NULL ? ctx->preset->name : NULL ),
        .env = args->env ? args->env : (ctx->env != NULL ? ctx->env->name : NULL),
        .toolchain = args->toolchain ? args->toolchain : (ctx->toolchain != NULL ? ctx->toolchain->name : NULL),
        .config = args->config ? args->config : (ctx->config != NULL ? ctx->config->name : NULL),
        .target = args->target ? args->target : (ctx->targets != NULL && fl_array_length(ctx->targets) == 1 ? ctx->targets[0]->name : NULL),
        .script_mode = (ctx->executor != NULL ? sbs_executor_is_script_mode(ctx->executor) : false), // TODO: Fix this
    }, &result);

    return copy;
}
