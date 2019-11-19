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

static bool run_actions(struct SbsBuild *build, struct SbsAction **actions)
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
            char *command = fl_cstring_dup(action->commands[j]);
            command = fl_cstring_replace_realloc(command, "${SBS_ENV_NAME}", build->env->name);
            command = fl_cstring_replace_realloc(command, "${SBS_CONFIG_NAME}", build->config->name);
            command = fl_cstring_replace_realloc(command, "${SBS_TARGET_NAME}", build->target->name);
            command = fl_cstring_replace_realloc(command, "${SBS_TOOLCHAIN_NAME}", build->toolchain->name);

            bool result = sbs_executor_run_command(build->executor, command);

            fl_cstring_free(command);

            if (!result)
                return false;
        }
    }

    return true;
}

static bool run_env_actions(enum ActionType type, struct SbsBuild *build)
{
    struct SbsAction **actions = type == ACTION_BEFORE ? build->env->actions.before : build->env->actions.after;

    if (!actions)
        return true;

    return run_actions(build, actions);
}

static bool run_target_actions(enum ActionType type, struct SbsBuild *build)
{
    if (!build->target)
        return true;

    struct SbsAction **actions = type == ACTION_BEFORE ? build->target->actions.before : build->target->actions.after;    

    if (!actions)
        return true;

    return run_actions(build, actions);
}

static bool run_preset_actions(enum ActionType type, struct SbsBuild *build)
{
    if (!build->preset)
        return true;

    struct SbsAction **actions = type == ACTION_BEFORE ? build->preset->actions.before : build->preset->actions.after;    

    if (!actions)
        return true;

    return run_actions(build, actions);
}


static enum SbsResult run_build_actions(enum ActionType type, struct SbsBuild *build)
{
    if (type == ACTION_BEFORE)
    {
        if (!run_preset_actions(ACTION_BEFORE, build))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(ACTION_BEFORE, build))
            return SBS_RES_ACTION_FAILED;

        if (!run_target_actions(ACTION_BEFORE, build))
            return SBS_RES_ACTION_FAILED;
    }
    else
    {
        if (!run_target_actions(ACTION_AFTER, build))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(ACTION_AFTER, build))
            return SBS_RES_ACTION_FAILED;
        
        if (!run_preset_actions(ACTION_AFTER, build))
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
    // Check if the build uses a preset and in that case make sure it is a valid preset
    struct SbsPreset *preset = NULL;
    if (args.preset)
    {
        preset = sbs_preset_resolve(file, args.preset, args.env);

        if (preset == NULL)
            return sbs_result_print_reason(SBS_RES_INVALID_PRESET, args.preset);      
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

    enum SbsResult result = SBS_RES_OK;

    struct SbsEnv *env = sbs_env_resolve(file, env_name);
    if (!env)
    {
        result = sbs_result_print_reason(SBS_RES_INVALID_ENV, env_name);
        goto env_error;
    }

    // Resolve toolchain
    struct SbsToolchain *toolchain = sbs_toolchain_resolve(file, toolchain_name, env_name);
    if (!toolchain)
    {
        result = sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
        goto toolchain_error;
    }

    // Resolve target
    struct SbsTarget *target = sbs_target_resolve(file, target_name, env_name);
    if (!target)
    {
        result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);
        goto target_error;
    }

    // Resolve configuration
    struct SbsConfiguration *config = sbs_config_resolve(file, configuration_name, env_name);
    if (!config)
    {
        result = sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);
        goto config_error;
    }

    // Create the executor
    SbsExecutor executor = sbs_executor_new(env);
    if (!executor)
    {
        result = sbs_result_print_reason(SBS_RES_ERROR);
        goto executor_error;
    }

    struct SbsBuild build = {
        .executor = executor, 
        .file = file, 
        .env = env, 
        .toolchain = toolchain,
        .target = target, 
        .config = config,
        .preset = preset
    };

    result = run_build_actions(ACTION_BEFORE, &build);

    if (result != SBS_RES_OK)
        goto action_before_failed;

    // Run the build
    char **target_output = sbs_build_target(&build);

    if (!target_output)
        build.target = NULL;

    // We run the target actions only if the build has been successful
    result = run_build_actions(ACTION_AFTER, &build);

    if (target_output)
        fl_array_free_each(target_output, sbs_common_free_string);

action_before_failed:
    sbs_executor_free(executor);
executor_error:
    sbs_config_free(config);
config_error:
    sbs_target_free(target);
target_error:
    sbs_toolchain_free(toolchain);
toolchain_error:
    sbs_env_free(env);
env_error:
    sbs_preset_free(preset);

    return SBS_RES_OK;
}
