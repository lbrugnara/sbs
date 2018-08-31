#ifndef SBS_BUILD_H
#define SBS_BUILD_H

#include <fllib.h>
#include "file.h"
#include "result.h"

struct SbsBuildArguments {
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    const char *file;
    const char *preset;
};

enum SbsResult sbs_build_run(const struct SbsFile *file, struct SbsBuildArguments arguments);

#endif /* SBS_BUILD_H */
