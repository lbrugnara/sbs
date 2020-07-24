#include <stdio.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "../result.h"
#include "../runtime/context.h"
#include "../runtime/resolvers/action.h"

static bool run_command(SbsContext *context, const char *actioncmd)
{
    // TODO: Implement proper string interpolation
    char *command = fl_cstring_dup(actioncmd);
    command = fl_cstring_replace_realloc(command, "${sbs.os}", sbs_host_os_to_str(context->host->os));
    command = fl_cstring_replace_realloc(command, "${sbs.arch}", sbs_host_arch_to_str(context->host->arch));
    command = fl_cstring_replace_realloc(command, "${sbs.env}", context->env->name);
    command = fl_cstring_replace_realloc(command, "${sbs.config}", context->config->name);
    command = fl_cstring_replace_realloc(command, "${sbs.target}", context->target->name);
    command = fl_cstring_replace_realloc(command, "${sbs.toolchain}", context->toolchain->name);

    bool result = sbs_executor_run_command(context->executor, command);

    fl_cstring_free(command);

    return result;
}

static bool run_actions(SbsContext *context, SbsValueCommand *actions)
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
            if (!run_command(context, action->value))
                return false;
        }
        else
        {
            SbsAction *action_obj = sbs_action_resolve(context, action->value);

            if (!action_obj)
                continue;

            bool result = true;
            for (size_t i=0; i < fl_array_length(action_obj->commands); i++)
            {
                if (!run_command(context, action_obj->commands[i]))
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

static bool run_env_actions(SbsContext *context, SbsBuildActionType type)
{
    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? context->env->actions.before : context->env->actions.after;

    if (!actions)
        return true;

    return run_actions(context, actions);
}

static bool run_target_actions(SbsContext *context, SbsBuildActionType type)
{
    if (!context->target)
        return true;

    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? context->target->actions.before : context->target->actions.after;    

    if (!actions)
        return true;

    return run_actions(context, actions);
}

static bool run_preset_actions(SbsContext *context, SbsBuildActionType type)
{
    if (!context->preset)
        return true;

    SbsValueCommand *actions = type == SBS_BUILD_ACTION_BEFORE ? context->preset->actions.before : context->preset->actions.after;    

    if (!actions)
        return true;

    return run_actions(context, actions);
}


SbsResult sbs_build_action_run(SbsContext *context, SbsBuildActionType type, bool run_target)
{
    if (type == SBS_BUILD_ACTION_BEFORE)
    {
        if (!run_preset_actions(context, SBS_BUILD_ACTION_BEFORE))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(context, SBS_BUILD_ACTION_BEFORE))
            return SBS_RES_ACTION_FAILED;

        if (run_target && !run_target_actions(context, SBS_BUILD_ACTION_BEFORE))
            return SBS_RES_ACTION_FAILED;
    }
    else
    {
        if (run_target && !run_target_actions(context, SBS_BUILD_ACTION_AFTER))
            return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(context, SBS_BUILD_ACTION_AFTER))
            return SBS_RES_ACTION_FAILED;
        
        if (!run_preset_actions(context, SBS_BUILD_ACTION_AFTER))
            return SBS_RES_ACTION_FAILED;
    }
    
    return SBS_RES_OK;
}
