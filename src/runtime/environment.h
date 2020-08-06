#ifndef SBS_RUNTIME_ENVIRONMENT_H
#define SBS_RUNTIME_ENVIRONMENT_H

#include "host.h"
#include "resolve.h"
#include "command.h"

typedef enum SbsShellType {
    SBS_SHELL_UNK,
    SBS_SHELL_SYSTEM,
    SBS_SHELL_BASH,
    SBS_SHELL_CMD,
    SBS_SHELL_POWERSHELL,
} SbsShellType;

typedef struct SbsEnv {
    SbsHostInfo *host;
    char *name;
    char *shell_command;
    char **shell_args;
    char **variables;
    struct {
        SbsCommand **before;
        SbsCommand **after;
    } actions;
    SbsOs os;
    SbsArch arch;
    SbsShellType shell_type;
} SbsEnv;

SbsEnv* sbs_env_new(const char *name);
void sbs_env_free(SbsEnv *env);
SbsEnv* sbs_env_resolve(SbsResolveContext *context, const char *env_name, const char *arch);

#endif /* SBS_RUNTIME_ENVIRONMENT_H */
