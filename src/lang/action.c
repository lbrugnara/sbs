#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "command.h"
#include "../utils.h"

static void action_node_free(SbsNodeAction *action_node)
{
    if (action_node->commands)
        fl_array_free_each(action_node->commands, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (action_node->condition)
        sbs_stmt_conditional_free(action_node->condition);

    fl_free(action_node);
}

void sbs_section_action_free(SbsSectionAction *action_section)
{
    fl_cstring_free(action_section->name);

    if (action_section->nodes)
        fl_array_free_each_pointer(action_section->nodes, (FlArrayFreeElementFunc) action_node_free);

    fl_free(action_section);
}

void sbs_property_actions_free(SbsPropertyActions *actions)
{
    if (actions->before)
        fl_array_free_each(actions->before, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (actions->after)
        fl_array_free_each(actions->after, (FlArrayFreeElementFunc) sbs_value_command_free);
}

SbsSectionAction* sbs_section_action_new(const struct FlSlice *name)
{
    SbsSectionAction *action_section = fl_malloc(sizeof(SbsSectionAction));

    action_section->name = sbs_slice_to_cstring(name);
    action_section->nodes = fl_array_new(sizeof(SbsNodeAction*), 0);

    return action_section;
}

SbsNodeAction* sbs_section_action_add_node(SbsSectionAction *action_section)
{
    SbsNodeAction *action_node = fl_malloc(sizeof(SbsNodeAction));
    action_node->commands = fl_array_new(sizeof(SbsValueCommand), 0);

    action_section->nodes = fl_array_append(action_section->nodes, &action_node);

    return action_node;
}
