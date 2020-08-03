#ifndef SBS_RUNTIME_TARGET_H
#define SBS_RUNTIME_TARGET_H

#include <fllib.h>

#include "resolve.h"
#include "../lang/action.h"
#include "../lang/target.h"

typedef struct SbsTarget {
    SbsSectionTargetType type;
    char *name;
    char *output_dir;
    SbsPropertyActions actions;
} SbsTarget;

SbsTarget* sbs_target_resolve(SbsResolveContext *context, const char *target_name, const SbsTarget *parent);
void sbs_target_free(SbsTarget *target);
void sbs_target_base_resolve_members(SbsTarget *extend, const SbsAbstractNodeTarget *source);
void sbs_target_base_free_members(SbsTarget *target);

#endif /* SBS_RUNTIME_TARGET_H */
