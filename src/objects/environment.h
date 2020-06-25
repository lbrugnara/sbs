#ifndef SBS_OBJECT_ENVIRONMENT_H
#define SBS_OBJECT_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

typedef struct {
    char *name;
    char *type;
    char *terminal;
    char **args;
    char **variables;
    SbsActions actions;
} SbsEnv;

SbsEnv* sbs_env_resolve(const SbsFile *file, const char *env_name);
void sbs_env_free(SbsEnv *env);

#endif /* SBS_OBJECT_ENVIRONMENT_H */
