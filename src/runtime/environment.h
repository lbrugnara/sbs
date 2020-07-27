#ifndef SBS_RUNTIME_ENVIRONMENT_H
#define SBS_RUNTIME_ENVIRONMENT_H

#include "host.h"
#include "../lang/action.h"

typedef struct SbsEnv {
    SbsHostInfo *host;
    char *name;
    char *type;
    char *terminal;
    char **args;
    char **variables;
    SbsPropertyActions actions;
    SbsHostOs os;
    SbsHostArch *arch;
} SbsEnv;

SbsEnv* sbs_env_new(const char *name);
void sbs_env_free(SbsEnv *env);

#endif /* SBS_RUNTIME_ENVIRONMENT_H */
