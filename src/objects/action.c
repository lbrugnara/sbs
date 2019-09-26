#include "action.h"

#include "../parser/action.h"

struct SbsAction* sbs_action_resolve(const struct SbsFile *file, const char *action_name, const char *env_name)
{
    struct SbsActionSection *action_section = fl_hashtable_get(file->actions, action_name);

    if (!action_section)
        return NULL;

    struct SbsAction *action_object = fl_malloc(sizeof(struct SbsAction));
    action_object->name = fl_cstring_dup(action_section->name);
    action_object->commands = fl_array_new(sizeof(char*), 0);

    for (size_t i=0; i < fl_array_length(action_section->nodes); i++)
    {
        struct SbsActionNode *ancestor = action_section->nodes + i;

        for (size_t i=0; i < fl_array_length(ancestor->commands); i++)
        {
            struct SbsStringOrId command = ancestor->commands[i];

            if (command.type == SBS_STRING)
            {
                size_t length = fl_array_length(action_object->commands);
                action_object->commands = fl_array_resize(action_object->commands, length + 1);
                action_object->commands[length] = fl_cstring_dup(command.value);
            }
            else
            {
                struct SbsAction *ref_action = sbs_action_resolve(file, command.value, env_name);
                
                if (!ref_action)
                {
                    fprintf(stderr, "Action '%s' does not exist (referenced in action '%s'", command.value, action_name);
                    continue;
                }

                action_object->commands = sbs_common_extend_array(action_object->commands, ref_action->commands);
                
                // Just the array, the elements now belongs to action_object->commands
                fl_array_free(ref_action->commands);

                if (ref_action->name)
                    fl_cstring_free(ref_action->name);

                fl_free(ref_action);
            }
        }
    }

    return action_object;
}

void sbs_action_free(struct SbsAction *action)
{
    if (action->name)
        fl_cstring_free(action->name);
    if (action->commands)
        fl_array_free_each(action->commands, sbs_common_free_string);

    fl_free(action);
}

struct SbsAction** sbs_action_resolve_all(const struct SbsFile *file, struct SbsStringOrId *actions, const char *env_name)
{
    if (!actions)
        return NULL;

    size_t actions_count = fl_array_length(actions);

    struct SbsAction **resolved_actions = fl_array_new(sizeof(struct SbsAction*), actions_count);

    for (size_t i=0; i < actions_count; i++)
    {
        struct SbsStringOrId action = actions[i];

        if (action.type == SBS_IDENTIFIER)
        {
            resolved_actions[i] = sbs_action_resolve(file, action.value, env_name);
        }
        else
        {
            struct SbsAction *action_object = fl_malloc(sizeof(struct SbsAction));
            action_object->commands = fl_array_new(sizeof(char*), 1);
            action_object->commands[0] = fl_cstring_dup(action.value);
            resolved_actions[i] = action_object;
        }
    }

    return resolved_actions;
}

void sbs_action_free_all(struct SbsAction **actions)
{
    if (!actions)
        return;

    for (size_t i=0; i < fl_array_length(actions); i++)
        sbs_action_free(actions[i]);

    fl_array_free(actions);
}
