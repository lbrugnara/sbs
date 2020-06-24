#ifndef SBS_OBJECT_ACTION_H
#define SBS_OBJECT_ACTION_H

#include <fllib.h>
#include "../common/common.h"
#include "../parser/common.h"
#include "../parser/file.h"

struct SbsAction {
    char *name;
    char **commands;
};

struct SbsActions {
    struct SbsAction **before;
    struct SbsAction **after;
};

struct SbsAction* sbs_action_resolve(const struct SbsFile *file, const char *action_name, const char *env_name);
void sbs_action_free(struct SbsAction *action);
struct SbsAction** sbs_action_resolve_all(const struct SbsFile *file, struct SbsStringOrId *actions, const char *env_name);
void sbs_action_free_all(struct SbsAction **actions);

#endif /* SBS_OBJECT_ACTION_H */
