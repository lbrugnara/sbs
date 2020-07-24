#ifndef SBS_RESOLVE_ACTION_H
#define SBS_RESOLVE_ACTION_H

#include "../action.h"
#include "../context.h"
#include "../../lang/command.h"

SbsAction* sbs_action_resolve(SbsContext *context, const char *action_name);
SbsAction** sbs_action_resolve_all(SbsContext *context, SbsValueCommand *actions);

#endif /* SBS_RESOLVE_ACTION_H */
