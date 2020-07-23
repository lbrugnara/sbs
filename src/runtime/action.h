#ifndef SBS_RUNTIME_ACTION_H
#define SBS_RUNTIME_ACTION_H

#include <fllib.h>
#include "context.h"
#include "../utils.h"
#include "../lang/command.h"
#include "../lang/file.h"
#include "../lang/for.h"

typedef struct SbsAction {
    char *name;
    char **commands;
} SbsAction;

SbsAction* sbs_action_resolve(SbsContext *context, const char *action_name);
SbsAction** sbs_action_resolve_all(SbsContext *context, SbsValueCommand *actions);
void sbs_action_free(SbsAction *action);
void sbs_action_free_all(SbsAction **actions);
void sbs_action_copy(SbsAction **dest, const SbsAction **src);

#endif /* SBS_RUNTIME_ACTION_H */
