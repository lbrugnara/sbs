#ifndef SBS_LANG_TARGET_SHARED_H
#define SBS_LANG_TARGET_SHARED_H

#include "target.h"

typedef struct SbsNodeShared {
    SbsAbstractNodeTarget base;
    const char *output_name;
    SbsValueSource *objects;
} SbsNodeShared;

typedef struct SbsSectionShared {
    SbsAbstractSectionTarget base;
    SbsNodeShared **entries;
} SbsSectionShared;

SbsSectionShared* sbs_section_shared_new(const struct FlSlice *name);
SbsNodeShared* sbs_section_shared_add_node(SbsSectionShared *target_section);
void sbs_node_shared_free(SbsNodeShared *node);
void sbs_section_shared_free(SbsSectionShared *section);

#endif /* SBS_LANG_TARGET_SHARED_H */
