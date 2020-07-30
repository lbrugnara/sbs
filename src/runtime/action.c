#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
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
