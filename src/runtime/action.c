#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "../lang/action.h"
#include "../utils.h"

SbsAction* sbs_action_new(const char *name)
{
    SbsAction *action = fl_malloc(sizeof(SbsAction));

    action->name = fl_cstring_dup(name);
    action->commands = fl_array_new(sizeof(SbsStringExpr*), 0);

    return action;
}

void sbs_action_add_command(SbsAction *action, SbsStringExpr *cmd)
{
    action->commands = fl_array_append(action->commands, &cmd);
}

void sbs_action_free(SbsAction *action)
{
    if (action->name)
        fl_cstring_free(action->name);

    if (action->commands)
        fl_array_free_each_pointer(action->commands, (FlArrayFreeElementFunc) sbs_expr_free);

    fl_free(action);
}

void sbs_action_free_all(SbsAction **actions)
{
    if (!actions)
        return;

    for (size_t i=0; i < fl_array_length(actions); i++)
        sbs_action_free(actions[i]);

    fl_array_free(actions);
}


void sbs_action_copy(SbsAction **dest, const SbsAction **src_action)
{
    if (!dest)
        return;

    if (!src_action || !*src_action)
    {
        memset(dest, 0, sizeof(SbsAction));
        return;
    }

    SbsAction *dst_action = fl_malloc(sizeof(SbsAction));

    dst_action->name = fl_cstring_dup((*src_action)->name);
    dst_action->commands = sbs_expr_extend_string_array(dst_action->commands, (*src_action)->commands);

    memcpy(dest, &dst_action, sizeof(SbsAction));
}


SbsAction* sbs_action_resolve(SbsResolveContext *context, const char *action_name)
{
    SbsSectionAction *action_section = fl_hashtable_get(context->file->actions, action_name);

    if (!action_section)
        return NULL;

    SbsAction *action_object = sbs_action_new(action_section->name);

    for (size_t i=0; i < fl_array_length(action_section->nodes); i++)
    {
        SbsNodeAction *action_node = action_section->nodes[i];

        if (action_node->condition && !sbs_eval_expr_to_bool(context->evalctx, action_node->condition))
            continue;

        for (size_t i=0; i < fl_array_length(action_node->commands); i++)
        {
            SbsCommand *command = action_node->commands[i];

            if (command->type == SBS_COMMAND_STRING)
            {
                sbs_action_add_command(action_object, (SbsStringExpr*) sbs_expr_copy((SbsExpression*) command->value.str));
            }
            else
            {
                char *ref_action_name = sbs_eval_expr_to_cstring(context->evalctx, (SbsExpression*) command->value.id);

                if (ref_action_name == NULL)
                    continue;

                SbsAction *ref_action = sbs_action_resolve(context, ref_action_name);
                
                if (!ref_action)
                {
                    fprintf(stderr, "Action '%s' does not exist (referenced in action '%s'", ref_action_name, action_name);
                }
                else
                {
                    action_object->commands = sbs_expr_extend_string_array(action_object->commands, ref_action->commands);
                    sbs_action_free(ref_action);
                }

                fl_cstring_free(ref_action_name);
            }
        }
    }

    return action_object;
}

SbsAction** sbs_action_resolve_all(SbsResolveContext *context, SbsCommand **actions)
{
    if (!actions)
        return NULL;

    size_t actions_count = fl_array_length(actions);

    SbsAction **resolved_actions = fl_array_new(sizeof(SbsAction*), actions_count);

    for (size_t i=0; i < actions_count; i++)
    {
        SbsCommand *action = actions[i];

        if (action->type == SBS_COMMAND_NAME)
        {
            char *ref_action_name = sbs_eval_expr_to_cstring(context->evalctx, (SbsExpression*) action->value.id);

            if (ref_action_name == NULL)
                continue;

            resolved_actions[i] = sbs_action_resolve(context, ref_action_name);
            fl_cstring_free(ref_action_name);
        }
        else
        {
            SbsAction *action_object = sbs_action_new(NULL);
            sbs_action_add_command(action_object, (SbsStringExpr*) sbs_expr_copy((SbsExpression*) action->value.str));
            resolved_actions[i] = action_object;
        }
    }

    return resolved_actions;
}
