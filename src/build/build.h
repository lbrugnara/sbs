#ifndef SBS_BUILD_H
#define SBS_BUILD_H

#include <fllib.h>
#include "../result.h"
#include "../host.h"
#include "../context.h"
#include "../executor.h"
#include "../parser/file.h"
#include "../objects/environment.h"
#include "../objects/configuration.h"
#include "../objects/preset.h"
#include "../objects/target.h"
#include "../objects/toolchain.h"

typedef struct SbsBuild {
    SbsContext *context;
    bool script_mode;
} SbsBuild;

SbsResult sbs_build_run(const SbsFile *file, SbsContextArgs *arguments);
char** sbs_build_target(SbsBuild *build);

#endif /* SBS_BUILD_H */
