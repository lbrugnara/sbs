#ifndef SBS_RUNTIME_TARGET_SHARED_H
#define SBS_RUNTIME_TARGET_SHARED_H

#include "target.h"
#include "resolve.h"
#include "../lang/target-shared.h"

typedef struct SbsTargetShared {
    SbsTarget base;
    char *output_name;
    SbsValueSource *objects;
} SbsTargetShared;

SbsTargetShared* sbs_target_shared_new(const char *name);
void sbs_target_shared_free(SbsTargetShared *target);
SbsTargetShared* sbs_target_shared_resolve(SbsResolveContext *context, const SbsSectionShared *shared_section);

#endif /* SBS_RUNTIME_TARGET_SHARED_H */
