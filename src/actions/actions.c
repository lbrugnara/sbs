#include <fllib.h>
#include "../parser/common.h"
#include "actions.h"

bool sbs_actions_run(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnvSection *env, struct SbsStringOrId *actions)
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
                struct SbsActionNode action_node = action->nodes[k];

                if (action_node.for_envs)
                {
                    bool should_run = false;
                    for (size_t i=0; i < fl_array_length(action_node.for_envs); i++)
                    {
                        if (flm_cstring_equals(action_node.for_envs[i], env->name))
                        {
                            should_run = true;
                            break;
                        }
                    }

                    if (!should_run)
                        continue;
                }

                if (action_node.commands)
                {
                    if (!sbs_actions_run(executor, file, env, action_node.commands))
                        return false;
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

bool sbs_actions_env_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, const struct SbsEnvSection *env)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? env->actions.before : env->actions.after;

    if (!actions)
        return true;

    return sbs_actions_run(executor, file, env, actions);
}

bool sbs_actions_target_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnvSection *env, struct SbsTarget *target)
{
    if (!target)
        return true;

    struct SbsAction **actions = type == ACTION_BEFORE ? target->actions.before : target->actions.after;    

    if (!actions)
        return true;

    struct SbsStringOrId *compat_actions = fl_array_new(sizeof(struct SbsStringOrId), 0);

    for (size_t i=0; i < fl_array_length(actions); i++)
    {
        for (size_t j=0; j < fl_array_length(actions[i]->commands); j++)
        {
            size_t length = fl_array_length(compat_actions);
            compat_actions = fl_array_resize(compat_actions, length + 1);        
            compat_actions[length].type = SBS_STRING;
            compat_actions[length].value = actions[i]->commands[j];
        }
    }

    bool result = sbs_actions_run(executor, file, env, compat_actions);

    fl_array_free(compat_actions);

    return result;
}

bool sbs_actions_preset_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnvSection *env, struct SbsPresetSection *preset)
{
    if (!preset)
        return true;

    struct SbsStringOrId *actions = type == ACTION_BEFORE ? preset->actions.before : preset->actions.after;

    if (!actions)
        return true;

    return sbs_actions_run(executor, file, env, actions);
}
