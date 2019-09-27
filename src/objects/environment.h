#ifndef SBS_OBJECT_ENVIRONMENT_H
#define SBS_OBJECT_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

struct SbsEnv {
    char *name;
    char *type;
    char *terminal;
    char **args;
    char **variables;
    struct SbsActions actions;
};

struct SbsEnv* sbs_env_resolve(const struct SbsFile *file, const char *env_name);
void sbs_env_free(struct SbsEnv *env);

#endif /* SBS_OBJECT_ENVIRONMENT_H */
