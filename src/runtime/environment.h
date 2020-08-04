#ifndef SBS_RUNTIME_ENVIRONMENT_H
#define SBS_RUNTIME_ENVIRONMENT_H

#include "host.h"
#include "resolve.h"
#include "command.h"

typedef struct SbsEnv {
    SbsHostInfo *host;
    char *name;
    char *type;
    char *terminal;
    char **args;
    char **variables;
    struct {
        SbsCommand **before;
        SbsCommand **after;
    } actions;
    SbsHostOs os;
    SbsHostArch *arch;
} SbsEnv;

SbsEnv* sbs_env_new(const char *name);
void sbs_env_free(SbsEnv *env);
SbsEnv* sbs_env_resolve(SbsResolveContext *context, const char *env_name);

#endif /* SBS_RUNTIME_ENVIRONMENT_H */
