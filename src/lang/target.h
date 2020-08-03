#ifndef SBS_LANG_TARGET_H
#define SBS_LANG_TARGET_H

#include <fllib.h>
#include "action.h"
#include "source.h"
#include "conditional.h"

typedef enum SbsSectionTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
} SbsSectionTargetType;

typedef struct SbsAbstractNodeTarget {
    SbsPropertyActions actions;
    const char *output_dir;
    SbsStmtConditional *condition;
} SbsAbstractNodeTarget;

typedef struct SbsAbstractSectionTarget {
    const char *name;
    SbsSectionTargetType type;
} SbsAbstractSectionTarget;

void sbs_node_target_free_members(SbsAbstractNodeTarget *node);
void sbs_section_target_free_members(SbsAbstractSectionTarget *section);
void sbs_section_target_free(SbsAbstractSectionTarget *section);

#endif /* SBS_LANG_TARGET_H */
