#ifndef SBS_BUILD_H
#define SBS_BUILD_H

#include <fllib.h>
#include "../result.h"
#include "../executor.h"
#include "../objects/file.h"
#include "../objects/environment.h"
#include "../objects/toolchain.h"
#include "../objects/target.h"
#include "../objects/configuration.h"

struct SbsBuildArguments {
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    const char *file;
    const char *preset;
};

struct SbsBuild {
    const SbsExecutor executor;
    const struct SbsFile *file;
    const struct SbsEnv *env;
    const struct SbsToolchainSection *toolchain;
    const struct SbsTarget *target;
    const struct SbsConfigSection *config;
};

enum SbsResult sbs_build_run(const struct SbsFile *file, struct SbsBuildArguments arguments);
FlVector sbs_build_target(struct SbsBuild *build);

#endif /* SBS_BUILD_H */
