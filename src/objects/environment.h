#ifndef SBS_OBJECT_ENVIRONMENT_H
#define SBS_OBJECT_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

struct SbsEnv {
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    struct SbsActionsNode actions;
};

void sbs_env_free(struct SbsEnv *env);
struct SbsEnv* sbs_env_parse(struct SbsParser *parser);

#endif /* SBS_OBJECT_ENVIRONMENT_H */
