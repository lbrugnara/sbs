#ifndef SBS_OBJECT_ACTION_H
#define SBS_OBJECT_ACTION_H

#include <fllib.h>
#include "../common/common.h"
#include "../parser/common.h"
#include "../parser/file.h"

typedef struct {
    char *name;
    char **commands;
} SbsAction;

typedef struct {
    SbsAction **before;
    SbsAction **after;
} SbsActions;

SbsAction* sbs_action_resolve(const SbsFile *file, const char *action_name, const char *env_name);
void sbs_action_free(SbsAction *action);
SbsAction** sbs_action_resolve_all(const SbsFile *file, SbsStringOrId *actions, const char *env_name);
void sbs_action_free_all(SbsAction **actions);

#endif /* SBS_OBJECT_ACTION_H */
