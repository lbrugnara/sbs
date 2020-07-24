#ifndef SBS_RUNTIME_CONTEXT_H
#define SBS_RUNTIME_CONTEXT_H

#include <stdbool.h>
#include "../result.h"
#include "../lang/file.h"
#include "configuration.h"
#include "eval.h"
#include "executor.h"
#include "environment.h"
#include "host.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"

typedef struct SbsContextArgs {
    char *env;
    char *toolchain;
    char *config;
    char *target;
    char *preset;
    bool script_mode;
} SbsContextArgs;

typedef struct SbsContext {
    const SbsFile *file;
    SbsHostInfo *host;
    SbsEvalContext *symbols;
    SbsExecutor *executor;
    SbsEnv *env;
    SbsToolchain *toolchain;
    SbsTarget *target;
    SbsConfiguration *config;
    SbsPreset *preset;
} SbsContext;

SbsContext* sbs_context_new(const SbsFile *file, SbsContextArgs *args, enum SbsResult *result);
void sbs_context_free(SbsContext *context);
SbsContext* sbs_context_copy(const SbsContext *ctx);

#endif /* SBS_RUNTIME_CONTEXT_H */
