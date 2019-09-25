#include <fllib.h>
#include "actions.h"

bool sbs_actions_run(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, struct SbsStringOrId *actions)
{
    if (!actions)
        return true;

    size_t nactions = fl_array_length(actions);
    for (size_t j=0; j < nactions; j++)
    {
        if (actions[j].type == SBS_IDENTIFIER)
        {
            struct SbsActionSection *action = fl_hashtable_get(file->actions, actions[j].value);

            if (!action || !action->nodes)
                continue;

            size_t countOfCommands = fl_array_length(action->nodes);
            for (size_t k=0; k < countOfCommands; k++)
            {
                struct SbsActionNode command = action->nodes[k];

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

bool sbs_actions_env_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? env->actions.before : env->actions.after;

    if (!actions)
        return true;

    return sbs_actions_run(executor, file, env, actions);
}

bool sbs_actions_target_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target)
{
    if (!target)
        return true;

    struct SbsStringOrId *actions = type == ACTION_BEFORE ? target->actions.before : target->actions.after;

    if (!actions)
        return true;

    return sbs_actions_run(executor, file, env, actions);
}

bool sbs_actions_preset_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsPreset *preset)
{
    if (!preset)
        return true;

    struct SbsStringOrId *actions = type == ACTION_BEFORE ? preset->actions.before : preset->actions.after;

    if (!actions)
        return true;

    return sbs_actions_run(executor, file, env, actions);
}
