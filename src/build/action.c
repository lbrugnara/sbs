#include <stdio.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "build.h"
#include "../result.h"
#include "../runtime/action.h"

static bool run_command(SbsBuild *build, const char *actioncmd)
{
    return sbs_executor_run_command(build->context->executor, actioncmd);
}

static bool run_actions(SbsBuild *build, SbsCommand **actions)
{
    if (!actions)
        return true;

    for (size_t i=0; i < fl_array_length(actions); i++)
    {
        SbsCommand *action = actions[i];
        
        if (!action)
            continue;

        if (action->type == SBS_COMMAND_STRING)
        {
            char *commandstr = sbs_eval_expr_string_to_cstring(build->context->evalctx, action->value.str);
            bool result = run_command(build, commandstr);
            fl_cstring_free(commandstr);

            if (!result)
                return false;
        }
        else
        {
            char *ref_action_name = sbs_eval_expr_to_cstring(build->context->evalctx, (SbsExpression*) action->value.id);

            if (ref_action_name == NULL)
                continue;

            SbsAction *action_obj = sbs_action_resolve(build->context->resolvectx, ref_action_name);

            fl_cstring_free(ref_action_name);

            if (!action_obj)
                continue;

            bool result = true;
            for (size_t i=0; i < fl_array_length(action_obj->commands); i++)
            {
                char *commandstr = sbs_eval_expr_string_to_cstring(build->context->evalctx, action_obj->commands[i]); 
                result = run_command(build, commandstr);
                fl_cstring_free(commandstr);

                if (!result)
                    break;
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
    SbsCommand **actions = type == SBS_BUILD_ACTION_BEFORE ? build->context->env->actions.before : build->context->env->actions.after;

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

    SbsCommand **actions = type == SBS_BUILD_ACTION_BEFORE ? build->current_target->actions.before : build->current_target->actions.after;    

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

    SbsCommand **actions = type == SBS_BUILD_ACTION_BEFORE ? build->context->preset->actions.before : build->context->preset->actions.after;    

    if (!actions)
        return SBS_RES_OK;

    if(run_actions(build, actions))
        return SBS_RES_OK;

    return SBS_RES_ACTION_FAILED;
}
