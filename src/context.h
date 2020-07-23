#ifndef SBS_CONTEXT_H
#define SBS_CONTEXT_H

#include <stdbool.h>
#include "result.h"

typedef struct SbsContextArgs {
    char *env;
    char *toolchain;
    char *config;
    char *target;
    char *preset;
    bool script_mode;
} SbsContextArgs;

typedef struct SbsContext {
    const struct SbsFile *file;
    struct SbsHostInfo *host;
    struct SbsForEvalContext *symbols;
    struct SbsExecutor *executor;
    struct SbsEnv *env;
    struct SbsToolchain *toolchain;
    struct SbsTarget *target;
    struct SbsConfiguration *config;
    struct SbsPreset *preset;
} SbsContext;

SbsContext* sbs_context_new(const struct SbsFile *file, SbsContextArgs *args, enum SbsResult *result);
void sbs_context_free(SbsContext *context);
SbsContext* sbs_context_copy(const SbsContext *ctx);

#endif /* SBS_CONTEXT_H */
