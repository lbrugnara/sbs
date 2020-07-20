#ifndef SBS_OBJECT_ENVIRONMENT_H
#define SBS_OBJECT_ENVIRONMENT_H

#include <fllib.h>
#include "../host.h"
#include "action.h"
#include "../context.h"

typedef struct SbsEnv {
    SbsHostInfo *host;
    char *name;
    char *type;
    char *terminal;
    char **args;
    char **variables;
    SbsActions actions;
    SbsHostOs os;
    SbsHostArch *arch;
} SbsEnv;

SbsEnv* sbs_env_resolve(SbsContext *context, const char *env_name);
void sbs_env_free(SbsEnv *env);

#endif /* SBS_OBJECT_ENVIRONMENT_H */
