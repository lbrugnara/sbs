#include <stdio.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "build.h"
#include "../result.h"
#include "../runtime/action.h"

static bool run_command(SbsBuild *build, const char *actioncmd)
{
    char *command = sbs_context_interpolate_string(build->context, actioncmd);

    bool result = sbs_executor_run_command(build->context->executor, command);

    fl_cstring_free(command);

    return result;
}

static bool run_actions(SbsBuild *build, SbsValueCommand *actions)
{
    if (!actions)
        return true;

    for (size_t i=0; i < fl_array_length(actions); i++)
    {
        SbsValueCommand *action = actions + i;
        
        if (!action)
            continue;

        if (action->type == SBS_COMMAND_STRING)
        {
            if (!run_command(build, action->value))
                return false;
        }
        else
        {
            SbsAction *action_obj = sbs_action_resolve(build->context->resolvectx, action->value);

            if (!action_obj)
                continue;

            bool result = true;
            for (size_t i=0; i < fl_array_length(action_obj->commands); i++)
            {
                if (!run_command(build, action_obj->commands[i]))
                {
                    result = false;
                    break;
                }
            }

            sbs_action_free(action_obj);
            
            if (!result)
                return false;
        }
    }

    return true;
}

SbsResult sbs_build_run_env_actions(SbsBuild *build, SbsBuildActionType type)
{
    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? build->context->env->actions.before : build->context->env->actions.after;

    if (!actions)
        return SBS_RES_OK;

    if(run_actions(build, actions))
        return SBS_RES_OK;

    return SBS_RES_ACTION_FAILED;
}

SbsResult sbs_build_run_target_actions(SbsBuild *build, SbsBuildActionType type)
{
    if (!build->current_target)
        return SBS_RES_OK;

    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? build->current_target->actions.before : build->current_target->actions.after;    

    if (!actions)
        return SBS_RES_OK;

    if(run_actions(build, actions))
        return SBS_RES_OK;

    return SBS_RES_ACTION_FAILED;
}

SbsResult sbs_build_run_preset_actions(SbsBuild *build, SbsBuildActionType type)
{
    if (!build->context->preset)
        return SBS_RES_OK;

    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? build->context->preset->actions.before : build->context->preset->actions.after;    

    if (!actions)
        return SBS_RES_OK;

    if(run_actions(build, actions))
        return SBS_RES_OK;

    return SBS_RES_ACTION_FAILED;
}
