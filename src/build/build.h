#ifndef SBS_BUILD_H
#define SBS_BUILD_H

#include <fllib.h>
#include "../result.h"
#include "../lang/file.h"
#include "../runtime/host.h"
#include "../runtime/context.h"
#include "../runtime/executor.h"
#include "../runtime/environment.h"
#include "../runtime/configuration.h"
#include "../runtime/preset.h"
#include "../runtime/target.h"
#include "../runtime/toolchain.h"

typedef struct SbsBuildArgs {
    char *env;
    char *toolchain;
    char *config;
    char *target;
    char *preset;
    bool script_mode;
    char *cwd;
} SbsBuildArgs;

typedef struct SbsBuild {
    SbsContext *context;
    bool script_mode;
    SbsTarget *current_target;
} SbsBuild;

SbsResult sbs_build_run(const SbsFile *file, SbsBuildArgs *arguments, char **env_vars);
char** sbs_build_target(SbsBuild *build);

#endif /* SBS_BUILD_H */
