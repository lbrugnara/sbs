#ifndef SBS_OBJECT_ACTION_H
#define SBS_OBJECT_ACTION_H

#include <fllib.h>
#include "../context.h"
#include "../common/common.h"
#include "../parser/helpers.h"
#include "../parser/file.h"
#include "../parser/for.h"

typedef struct SbsAction {
    char *name;
    char **commands;
} SbsAction;

typedef struct SbsActions {
    SbsStringOrId *before;
    SbsStringOrId *after;
} SbsActions;

SbsAction* sbs_action_resolve(SbsContext *context, const char *action_name);
SbsAction** sbs_action_resolve_all(SbsContext *context, SbsStringOrId *actions);
void sbs_action_free(SbsAction *action);
void sbs_action_free_all(SbsAction **actions);
void sbs_action_copy(SbsAction **dest, const SbsAction **src);
//void sbs_action_command_copy(SbsActionEntry **dest, const SbsActionEntry **src);

#endif /* SBS_OBJECT_ACTION_H */
