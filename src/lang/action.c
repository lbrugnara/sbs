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

    if (action_node->for_clause)
        sbs_section_for_free(action_node->for_clause);

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
