#include <stdio.h>
#include <fllib.h>

#include "build.h"
#include "archive.h"
#include "compile.h"
#include "../executor.h"
#include "../actions/actions.h"
#include "../objects/file.h"
#include "../objects/action.h"
#include "../objects/common.h"
#include "../objects/configuration.h"
#include "../objects/environment.h"
#include "../objects/preset.h"
#include "../objects/target.h"
#include "../objects/toolchain.h"
#include "../objects/lexer.h"
#include "../objects/parser.h"
#include "../objects/file.h"


static enum SbsResult run_build_actions(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target, struct SbsPreset *preset)
{
    if (type == ACTION_BEFORE)
    {
        if (!sbs_actions_preset_run(ACTION_BEFORE, executor, file, env, preset))
            return SBS_RES_ACTION_FAILED;

        if (!sbs_actions_env_run(ACTION_BEFORE, executor, file, env))
            return SBS_RES_ACTION_FAILED;

        if (!sbs_actions_target_run(ACTION_BEFORE, executor, file, env, target))
            return SBS_RES_ACTION_FAILED;
    }
    else
    {
        if (!sbs_actions_target_run(ACTION_AFTER, executor, file, env, target))
            return SBS_RES_ACTION_FAILED;

        if (!sbs_actions_env_run(ACTION_AFTER, executor, file, env))
            return SBS_RES_ACTION_FAILED;
        
        if (!sbs_actions_preset_run(ACTION_AFTER, executor, file, env, preset))
            return SBS_RES_ACTION_FAILED;
    }
    
    return SBS_RES_OK;
}

char** sbs_build_target(struct SbsBuild *build)
{
    if (build->target->output_dir && !fl_io_file_exists(build->target->output_dir))
    {
        if (!fl_io_dir_create_recursive(build->target->output_dir))
            printf("Failed to create directory %s\n", build->target->output_dir);
    }
    
    if (build->target->type == SBS_TARGET_COMPILE)
    {
        return sbs_build_compile(build);
    }
    else if (build->target->type == SBS_TARGET_ARCHIVE)
    {
        return sbs_build_target_archive(build);
    }
    else if (build->target->type == SBS_TARGET_EXECUTABLE)
    {
        struct SbsTargetExecutableNode *exe = (struct SbsTargetExecutableNode*)build->target;
    }

    return NULL;
}

enum SbsResult sbs_build_run(const struct SbsFile *file, struct SbsBuildArguments args)
{
    // Check if the build uses a preset and in that case make sure it is a valid preset
    struct SbsPreset *preset = NULL;
    if (args.preset)
    {
        preset = fl_hashtable_get(file->presets, args.preset);

        if (preset == NULL)
            return sbs_result_print_reason(SBS_RES_INVALID_PRESET, args.preset);
    }

    // Get the resources from the preset or from the arguments list
    const char *env_name = preset && preset->env ? preset->env : args.env;
    const char *toolchain_name = preset && preset->toolchain ? preset->toolchain : args.toolchain;
    const char *target_name = preset && preset->target ? preset->target : args.target;
    const char *configuration_name = preset && preset->config ? preset->config : args.config;

    // If one of them is not present we can't continue
    if (!env_name)
        return sbs_result_print_reason(SBS_RES_MISSING_ENV_ARG);
    
    if (!toolchain_name)
        return sbs_result_print_reason(SBS_RES_MISSING_TOOLCHAIN_ARG);

    if (!target_name)
        return sbs_result_print_reason(SBS_RES_MISSING_TARGET_ARG);
    
    if (!configuration_name)
        return sbs_result_print_reason(SBS_RES_MISSING_CONFIG_ARG);

    // Check if the resources exist
    if (!fl_hashtable_has_key(file->toolchains, toolchain_name))
        return sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
    
    if (!fl_hashtable_has_key(file->configurations, configuration_name))
        return sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);

    struct SbsEnv *env = fl_hashtable_get(file->envs, env_name);
    if (!env) 
        return sbs_result_print_reason(SBS_RES_INVALID_ENV, env_name);

    struct SbsToolchainSection *toolchain = fl_hashtable_get(file->toolchains, toolchain_name);
    if (!toolchain) 
        return sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
    
    defer_scope {

        struct SbsTarget *target = sbs_target_resolve(target_name, file->targets, env_name);
        if (!target) 
            return sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);

        defer_expression(sbs_target_release(target));

        struct SbsConfiguration *config = sbs_config_resolve(configuration_name, file->configurations, env_name);
        if (!config)
            defer_return sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);

        defer_expression(sbs_config_release(config));

        // Create the executor
        SbsExecutor executor = sbs_executor_new(env);
        defer_expression(sbs_executor_free(executor));

        enum SbsResult actions_result = SBS_RES_OK;

        actions_result = run_build_actions(ACTION_BEFORE, executor, file, env, target, preset);
        if (actions_result != SBS_RES_OK)
            defer_return actions_result;

        // Run the build
        char **target_output = sbs_build_target(&(struct SbsBuild) {
            .executor = executor, 
            .file = file, 
            .env = env, 
            .toolchain = toolchain,
            .target = target, 
            .config = config
        });
        if (target_output != NULL)
            defer_expression(fl_array_free_each(target_output, sbs_common_free_string));

        // We run the target actions only if the build has been successful
        actions_result = run_build_actions(ACTION_AFTER, executor, file, env, target_output != NULL ? target : NULL, preset);
        if (actions_result != SBS_RES_OK)
            defer_return actions_result;
    }

    return SBS_RES_OK;
}
