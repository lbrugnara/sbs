#ifndef SBS_RUNTIME_TARGET_COMPILE_H
#define SBS_RUNTIME_TARGET_COMPILE_H

#include "target.h"
#include "resolve.h"
#include "../lang/target-compile.h"

typedef struct SbsTargetCompile {
    SbsTarget base;
    char **includes;
    char **sources;
    char **defines;
} SbsTargetCompile;

SbsTargetCompile* sbs_target_compile_new(const char *name);
void sbs_target_compile_free(SbsTargetCompile *target);
SbsTargetCompile* sbs_target_compile_resolve(SbsResolveContext *context, const SbsSectionCompile *compile_section, const SbsTarget *parent);

#endif /* SBS_RUNTIME_TARGET_COMPILE_H */
