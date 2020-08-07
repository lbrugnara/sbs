#ifndef SBS_RUNTIME_ACTION_H
#define SBS_RUNTIME_ACTION_H

#include "resolve.h"
#include "../lang/string.h"
#include "../lang/command.h"

typedef struct SbsAction {
    char *name;
    SbsStringExpr **commands;
} SbsAction;

SbsAction* sbs_action_new(const char *name);
void sbs_action_free(SbsAction *action);
void sbs_action_free_all(SbsAction **actions);
void sbs_action_add_command(SbsAction *action, SbsStringExpr *cmd);
void sbs_action_copy(SbsAction **dest, const SbsAction **src);

SbsAction* sbs_action_resolve(SbsResolveContext *context, const char *action_name);
SbsAction** sbs_action_resolve_all(SbsResolveContext *context, SbsCommand **actions);

#endif /* SBS_RUNTIME_ACTION_H */
