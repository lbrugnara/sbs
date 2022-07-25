#ifndef SBS_RUNTIME_TARGET_H
#define SBS_RUNTIME_TARGET_H

#include <fllib.h>

#include "resolve.h"
#include "../lang/command.h"
#include "../lang/target.h"

typedef enum SbsTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
} SbsTargetType;

typedef struct SbsTarget {
    SbsTargetType type;
    char *name;
    char *output_dir;
    struct {
        SbsCommand **before;
        SbsCommand **after;
    } actions;
    SbsStringExpr **flags;
} SbsTarget;

SbsTarget* sbs_target_resolve(SbsResolveContext *context, const char *target_name, const SbsTarget *parent);
void sbs_target_free(SbsTarget *target);
void sbs_target_base_resolve_members(SbsTarget *extend, const SbsAbstractNodeTarget *source);
void sbs_target_base_free_members(SbsTarget *target);

#endif /* SBS_RUNTIME_TARGET_H */
