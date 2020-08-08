#ifndef SBS_LANG_TARGET_ARCHIVE_H
#define SBS_LANG_TARGET_ARCHIVE_H

#include "target.h"

typedef struct SbsNodeArchive {
    SbsAbstractNodeTarget base;
    const char *output_name;
    SbsSource **objects;
} SbsNodeArchive;

typedef struct SbsSectionArchive {
    SbsAbstractSectionTarget base;
    SbsNodeArchive **entries;
} SbsSectionArchive;

void sbs_section_archive_body_parse(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target);
SbsAbstractSectionTarget* sbs_section_archive_parse(SbsParser *parser);
SbsSectionArchive* sbs_section_archive_new(const struct FlSlice *name);
SbsNodeArchive* sbs_section_archive_add_node(SbsSectionArchive *section);
void sbs_node_archive_free(SbsNodeArchive *node);
void sbs_section_archive_free(SbsSectionArchive *section);

#endif /* SBS_LANG_TARGET_ARCHIVE_H */
