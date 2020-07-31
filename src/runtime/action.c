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
    action->commands = fl_array_new(sizeof(char*), 0);

    return action;
}

void sbs_action_add_command(SbsAction *action, char *cmd)
{
    action->commands = fl_array_append(action->commands, &cmd);
}

void sbs_action_free(SbsAction *action)
{
    if (action->name)
        fl_cstring_free(action->name);

    if (action->commands)
        fl_array_free_each_pointer(action->commands, (FlArrayFreeElementFunc) fl_cstring_free);

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
    dst_action->commands = sbs_cstring_array_extend(dst_action->commands, (*src_action)->commands);

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

        if (action_node->condition && !sbs_expression_eval_bool(context->symbols, action_node->condition->expr))
            continue;

        for (size_t i=0; i < fl_array_length(action_node->commands); i++)
        {
            SbsValueCommand command = action_node->commands[i];

            if (command.type == SBS_COMMAND_STRING)
            {
                sbs_action_add_command(action_object, fl_cstring_dup(command.value));
            }
            else
            {
                SbsAction *ref_action = sbs_action_resolve(context, command.value);
                
                if (!ref_action)
                {
                    fprintf(stderr, "Action '%s' does not exist (referenced in action '%s'", command.value, action_name);
                    continue;
                }

                action_object->commands = sbs_cstring_array_extend(action_object->commands, ref_action->commands);

                sbs_action_free(ref_action);
            }
        }
    }

    return action_object;
}

SbsAction** sbs_action_resolve_all(SbsResolveContext *context, SbsValueCommand *actions)
{
    if (!actions)
        return NULL;

    size_t actions_count = fl_array_length(actions);

    SbsAction **resolved_actions = fl_array_new(sizeof(SbsAction*), actions_count);

    for (size_t i=0; i < actions_count; i++)
    {
        SbsValueCommand action = actions[i];

        if (action.type == SBS_COMMAND_NAME)
        {
            resolved_actions[i] = sbs_action_resolve(context, action.value);
        }
        else
        {
            SbsAction *action_object = sbs_action_new(NULL);
            sbs_action_add_command(action_object, fl_cstring_dup(action.value));
            resolved_actions[i] = action_object;
        }
    }

    return resolved_actions;
}
