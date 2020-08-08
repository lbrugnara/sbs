#ifndef SBS_LANG_TARGET_H
#define SBS_LANG_TARGET_H

#include <fllib.h>
#include "action.h"
#include "source.h"
#include "expr.h"

typedef enum SbsSectionTargetType {
    SBS_SECTION_TARGET_COMPILE,
    SBS_SECTION_TARGET_ARCHIVE,
    SBS_SECTION_TARGET_SHARED,
    SBS_SECTION_TARGET_EXECUTABLE,
} SbsSectionTargetType;

typedef struct SbsAbstractNodeTarget {
    SbsPropertyActions actions;
    const char *output_dir;
    SbsExpression *condition;
} SbsAbstractNodeTarget;

typedef struct SbsAbstractSectionTarget {
    const char *name;
    SbsSectionTargetType type;
} SbsAbstractSectionTarget;

SbsAbstractSectionTarget* sbs_section_target_parse(SbsParser *parser);
void sbs_section_target_if_stmt_parse(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type);
void sbs_section_target_free_members(SbsAbstractSectionTarget *section);
void sbs_section_target_free(SbsAbstractSectionTarget *section);
void sbs_node_target_free_members(SbsAbstractNodeTarget *node);

#endif /* SBS_LANG_TARGET_H */
