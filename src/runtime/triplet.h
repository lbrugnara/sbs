#ifndef SBS_RUNTIME_TRIPLET_H
#define SBS_RUNTIME_TRIPLET_H

#include "../lang/file.h"
#include "context.h"
#include "environment.h"
#include "toolchain.h"
#include "configuration.h"

typedef struct SbsTriplet {
    SbsContext *context;
} SbsTriplet;

SbsTriplet** sbs_triplet_find(const SbsFile *file, const char *preset, const char *env, const char *toolchain, const char *config, bool script_mode);
SbsTriplet* sbs_triplet_new(SbsContext *context);
void sbs_triplet_free(SbsTriplet *triplet);
FlList* sbs_triplet_resolve_all(SbsContext *context, const char *env, const char *toolchain, const char *config, bool script_mode);

#endif /* SBS_RUNTIME_TRIPLET_H */
