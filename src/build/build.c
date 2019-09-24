#include <stdio.h>
#include <fllib.h>

#include "build.h"
#include "archive.h"
#include "compile.h"
#include "../executor.h"
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

enum ActionType {
    ACTION_BEFORE,
    ACTION_AFTER
};

static bool run_actions(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, struct SbsStringOrId *actions)
{
    if (!actions)
        return true;

    size_t nactions = fl_array_length(actions);
    for (size_t j=0; j < nactions; j++)
    {
        if (actions[j].type == SBS_IDENTIFIER)
        {
            struct SbsAction *action = fl_hashtable_get(file->actions, actions[j].value);

            if (!action || !action->commands)
                continue;

            size_t countOfCommands = fl_array_length(action->commands);
            for (size_t k=0; k < countOfCommands; k++)
            {
                struct SbsActionCommand command = action->commands[k];

                if (command.for_envs)
                {
                    bool should_run = false;
                    for (size_t i=0; i < fl_array_length(command.for_envs); i++)
                    {
                        if (flm_cstring_equals(command.for_envs[i], env->name))
                        {
                            should_run = true;
                            break;
                        }
                    }

                    if (!should_run)
                        continue;
                }

                if (command.commands)
                {
                    for (size_t i=0; i < fl_array_length(command.commands); i++)
                    {
                        if (!sbs_executor_run_command(executor, command.commands[i]))
                            return false;
                    }
                }
            }
        }
        else
        {
            if (!sbs_executor_run_command(executor, actions[j].value))
                return false;
        }
    }

    return true;
}

bool run_env_actions(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, enum ActionType type)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? env->actions.before : env->actions.after;
    return run_actions(executor, file, env, actions);
}

bool run_target_actions(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target, enum ActionType type)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? target->actions.before : target->actions.after;
    return run_actions(executor, file, env, actions);
}


FlVector sbs_build_target(struct SbsBuild *build)
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
        struct SbsTargetExecutable *exe = (struct SbsTargetExecutable*)build->target;
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

    struct SbsTarget *target = fl_hashtable_get(file->targets, target_name);
    if (!target) 
        return sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);

    struct SbsToolchainSection *toolchain = fl_hashtable_get(file->toolchains, toolchain_name);
    if (!toolchain) 
        return sbs_result_print_reason(SBS_RES_INVALID_TOOLCHAIN, toolchain_name);
    
    // The config needs to be processed and deleted after its usage
    struct SbsConfigSection *config = sbs_config_resolve(file->configurations, configuration_name, env_name);
    if (!config)
        return sbs_result_print_reason(SBS_RES_INVALID_CONFIG, configuration_name);

    defer_scope 
    {
        defer_expression(sbs_config_delete(config));

        // Create the executor
        SbsExecutor executor = sbs_executor_create(env);
        defer_expression(sbs_executor_delete(executor));

        // If we are using a preset, check if there are "before" actions to run
        if (preset && preset->actions.before)
            if (!run_actions(executor, file, env, preset->actions.before))
                defer_return SBS_RES_ACTION_FAILED;

        // Run environment's "before" actions if present
        if (!run_env_actions(executor, file, env, ACTION_BEFORE))
            defer_return SBS_RES_ACTION_FAILED;

        // Run target's "before" actions
        if (!run_target_actions(executor, file, env, target, ACTION_BEFORE))
            defer_return SBS_RES_ACTION_FAILED;

        // Run the build
        FlVector vector = sbs_build_target(&(struct SbsBuild) {
            .executor = executor, 
            .file = file, 
            .env = env, 
            .toolchain = toolchain,
            .target = target, 
            .config = config
        });
        if (vector != NULL)
            defer_expression(fl_vector_delete(vector));

        // Run target's "after" actions if the target has been successful
        if (vector != NULL && !run_target_actions(executor, file, env, target, ACTION_AFTER))
            defer_return SBS_RES_ACTION_FAILED;

        // Run environment's "after" actions if present
        if (!run_env_actions(executor, file, env, ACTION_AFTER))
            defer_return SBS_RES_ACTION_FAILED;        

        // If we are using a preset, check if there are "after" actions to run
        if (preset && preset->actions.after)
            if (!run_actions(executor, file, env, preset->actions.after))
                defer_return SBS_RES_ACTION_FAILED;
    }

    return SBS_RES_OK;
}
