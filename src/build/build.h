#ifndef SBS_BUILD_H
#define SBS_BUILD_H

#include <fllib.h>
#include "../common/result.h"
#include "../executor.h"
#include "../parser/file.h"
#include "../objects/environment.h"
#include "../objects/configuration.h"
#include "../objects/preset.h"
#include "../objects/target.h"
#include "../objects/toolchain.h"

typedef struct {
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    const char *file;
    const char *preset;
} SbsBuildArguments;

typedef struct {
    const SbsExecutor *executor;
    const SbsFile *file;
    const SbsEnv *env;
    const SbsToolchain *toolchain;
    const SbsTarget *target;
    const SbsConfiguration *config;
    const SbsPreset *preset;
} SbsBuild;

SbsResult sbs_build_run(const SbsFile *file, SbsBuildArguments arguments);
char** sbs_build_target(SbsBuild *build);

#endif /* SBS_BUILD_H */
