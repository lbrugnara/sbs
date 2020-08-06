#ifndef SBS_RUNTIME_RESOLVE_H
#define SBS_RUNTIME_RESOLVE_H

#include <stdbool.h>
#include "eval.h"
#include "host.h"
#include "../lang/file.h"

typedef struct SbsResolveContext {
    const SbsFile *file;
    const SbsHostInfo *host;
    SbsEvalContext *evalctx;
    bool script_mode;
} SbsResolveContext;

SbsResolveContext* sbs_resolve_context_new(const SbsFile *file, const SbsHostInfo *host, SbsEvalContext *evalctx);
void sbs_resolve_context_free(SbsResolveContext *context);

#endif /* SBS_RUNTIME_RESOLVE_H */
