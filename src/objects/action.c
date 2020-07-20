#include <fllib.h>
#include "action.h"
#include "../parser/action.h"

SbsAction* sbs_action_resolve(SbsContext *context, const char *action_name)
{
    SbsSectionAction *action_section = fl_hashtable_get(context->file->actions, action_name);

    if (!action_section)
        return NULL;

    SbsAction *action_object = fl_malloc(sizeof(SbsAction));
    action_object->name = fl_cstring_dup(action_section->name);
    action_object->commands = fl_array_new(sizeof(char*), 0);

    for (size_t i=0; i < fl_array_length(action_section->nodes); i++)
    {
        SbsNodeAction *action_node = action_section->nodes[i];

        if (action_node->for_clause && !sbs_for_node_eval(action_node->for_clause->condition, context->symbols))
            continue;

        for (size_t i=0; i < fl_array_length(action_node->commands); i++)
        {
            SbsStringOrId command = action_node->commands[i];

            if (command.type == SBS_STRING)
            {
                char *cmd = fl_cstring_dup(command.value);
                action_object->commands = fl_array_append(action_object->commands, &cmd);
            }
            else
            {
                SbsAction *ref_action = sbs_action_resolve(context, command.value);
                
                if (!ref_action)
                {
                    fprintf(stderr, "Action '%s' does not exist (referenced in action '%s'", command.value, action_name);
                    continue;
                }

                action_object->commands = sbs_common_extend_array_copy(action_object->commands, ref_action->commands, (SbsArrayCopyElementFn) sbs_common_copy_string);

                sbs_action_free(ref_action);
            }
        }
    }

    return action_object;
}

void sbs_action_free(SbsAction *action)
{
    if (action->name)
        fl_cstring_free(action->name);

    if (action->commands)
        fl_array_free_each_pointer(action->commands, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(action);
}

SbsAction** sbs_action_resolve_all(SbsContext *context, SbsStringOrId *actions)
{
    if (!actions)
        return NULL;

    size_t actions_count = fl_array_length(actions);

    SbsAction **resolved_actions = fl_array_new(sizeof(SbsAction*), actions_count);

    for (size_t i=0; i < actions_count; i++)
    {
        SbsStringOrId action = actions[i];

        if (action.type == SBS_IDENTIFIER)
        {
            resolved_actions[i] = sbs_action_resolve(context, action.value);
        }
        else
        {
            SbsAction *action_object = fl_malloc(sizeof(SbsAction));
            action_object->commands = fl_array_new(sizeof(char*), 1);
            action_object->commands[0] = fl_cstring_dup(action.value);
            resolved_actions[i] = action_object;
        }
    }

    return resolved_actions;
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
    dst_action->commands = sbs_common_extend_array_copy(dst_action->commands, (*src_action)->commands, (SbsArrayCopyElementFn) sbs_common_copy_string);

    memcpy(dest, &dst_action, sizeof(SbsAction));
}
