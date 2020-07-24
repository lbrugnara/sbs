#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "action.h"
#include "../../lang/action.h"

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

        if (action_node->for_clause && !sbs_expression_eval(context->symbols, action_node->for_clause->expr))
            continue;

        for (size_t i=0; i < fl_array_length(action_node->commands); i++)
        {
            SbsValueCommand command = action_node->commands[i];

            if (command.type == SBS_COMMAND_STRING)
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

                action_object->commands = sbs_string_array_extend(action_object->commands, ref_action->commands);

                sbs_action_free(ref_action);
            }
        }
    }

    return action_object;
}

SbsAction** sbs_action_resolve_all(SbsContext *context, SbsValueCommand *actions)
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
            SbsAction *action_object = fl_malloc(sizeof(SbsAction));
            action_object->commands = fl_array_new(sizeof(char*), 1);
            action_object->commands[0] = fl_cstring_dup(action.value);
            resolved_actions[i] = action_object;
        }
    }

    return resolved_actions;
}
