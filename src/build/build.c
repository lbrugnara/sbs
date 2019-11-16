#include <stdio.h>
#include <fllib.h>

#include "build.h"
#include "archive.h"
#include "compile.h"
#include "executable.h"
#include "shared.h"
#include "../common.h"
#include "../executor.h"
#include "../parser/file.h"
#include "../parser/action.h"
#include "../parser/common.h"
#include "../parser/configuration.h"
#include "../parser/target.h"
#include "../parser/toolchain.h"
#include "../parser/lexer.h"
#include "../parser/parser.h"
#include "../parser/file.h"

#include "../objects/configuration.h"
#include "../objects/toolchain.h"
#include "../objects/preset.h"


enum ActionType {
    ACTION_BEFORE,
    ACTION_AFTER
};

static bool run_actions(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, struct SbsAction **actions)
{
    if (!actions)
        return true;

    for (size_t i=0; i < fl_array_length(actions); i++)
    {
        struct SbsAction *action = actions[i];
        
        if (!action)
            return false;

        for (size_t j=0; j < fl_array_length(action->commands); j++)
        {
            if (!sbs_executor_run_command(executor, action->commands[j]))
                return false;
        }
    }

    return true;
}

static bool run_env_actions(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env)
{
    struct SbsAction **actions = type == ACTION_BEFORE ? env->actions.before : env->actions.after;

    if (!actions)
        return true;

    return run_actions(executor, file, env, actions);
}

static bool run_target_actions(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target)
{
    if (!target)
        return true;

    struct SbsAction **actions = type == ACTION_BEFORE ? target->actions.before : target->actions.after;    

    if (!actions)
        return true;

    return run_actions(executor, file, env, actions);
}

static bool run_preset_actions(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsPreset *preset)
{
    if (!preset)
        return true;

    struct SbsAction **actions = type == ACTION_BEFORE ? preset->actions.before : preset->actions.after;    

    if (!actions)
        return true;

    return run_actions(executor, file, env, actions);
}


static enum SbsResult run_build_actions(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target, struct SbsPreset *preset)
{
    if (type == ACTION_BEFORE)
    {
        if (!run_preset_actions(ACTION_BEFORE, executor, file, env, preset))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(ACTION_BEFORE, executor, file, env))
            return SBS_RES_ACTION_FAILED;

        if (!run_target_actions(ACTION_BEFORE, executor, file, env, target))
            return SBS_RES_ACTION_FAILED;
    }
    else
    {
        if (!run_target_actions(ACTION_AFTER, executor, file, env, target))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(ACTION_AFTER, executor, file, env))
            return SBS_RES_ACTION_FAILED;
        
        if (!run_preset_actions(ACTION_AFTER, executor, file, env, preset))
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
    else if (build->target->type == SBS_TARGET_SHARED)
    {
        return sbs_build_target_shared(build);
    }
    else if (build->target->type == SBS_TARGET_EXECUTABLE)
    {
        return sbs_build_target_executable(build);
    }

    return NULL;
}

enum SbsResult sbs_build_run(const struct SbsFile *file, struct SbsBuildArguments args)
{
    defer_scope
    {
        // Check if the build uses a preset and in that case make sure it is a valid preset
        struct SbsPreset *preset = NULL;
        if (args.preset)
        {
            preset = sbs_preset_resolve(file, args.preset, args.env);

            if (preset == NULL)
                return sbs_result_print_reason(SBS_RES_INVALID_PRESET, args.preset);      
            defer_expression(sbs_preset_free(preset));
        }

        // Get the resources from the preset or from the arguments list. The preset's values
        // can be overwritten
        const char *env_name = preset && preset->env ? preset->env : NULL;
        if (args.env)
            env_name = args.env;

        const char *toolchain_name = preset && preset->toolchain ? preset->toolchain : NULL;
        if (args.toolchain)
            toolchain_name = args.toolchain;

        const char *target_name = preset && preset->target ? preset->target : NULL;
        if (args.target)
            target_name = args.target;

        const char *configuration_name = preset && preset->config ? preset->config : NULL;
        if (args.config)
            configuration_name = args.config;


        // If one of them is not present we can't continue
        if (!env_name)
            defer_return sbs_result_print_reason(SBS_RES_MISSING_ENV_ARG);
        
        if (!toolchain_name)
            defer_return sbs_result_print_reason(SBS_RES_MISSING_TOOLCHAIN_ARG);

        if (!target_name)
            defer_return sbs_result_print_reason(SBS_RES_MISSING_TARGET_ARG);
        
        if (!configuration_name)
            defer_return sbs_result_print_reason(SBS_RES_MISSING_CONFIG_ARG);

        // Check if the resources exist
        if (!fl_hashtable_has_key(file->toolchains, toolchain_name))
            defer_return sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
        
        if (!fl_hashtable_has_key(file->configurations, configuration_name))
            defer_return sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);

        struct SbsEnv *env = sbs_env_resolve(file, env_name);
        if (!env) 
            defer_return sbs_result_print_reason(SBS_RES_INVALID_ENV, env_name);
        defer_expression(sbs_env_free(env));

        // Resolve toolchain
        struct SbsToolchain *toolchain = sbs_toolchain_resolve(file, toolchain_name, env_name);
        if (!toolchain) 
            defer_return sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
        defer_expression(sbs_toolchain_free(toolchain));

        // Resolve target
        struct SbsTarget *target = sbs_target_resolve(file, target_name, env_name);
        if (!target) 
            defer_return sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);
        defer_expression(sbs_target_free(target));

        // Resolve configuration
        struct SbsConfiguration *config = sbs_config_resolve(file, configuration_name, env_name);
        if (!config)
            defer_return sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);
        defer_expression(sbs_config_free(config));

        // Create the executor
        SbsExecutor executor = sbs_executor_new(env);
        if (!executor)
            defer_return sbs_result_print_reason(SBS_RES_ERROR);
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
