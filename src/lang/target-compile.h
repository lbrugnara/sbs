#ifndef SBS_LANG_TARGET_COMPILE_H
#define SBS_LANG_TARGET_COMPILE_H

#include "target.h"
#include "parser.h"

typedef struct SbsNodeCompile {
    SbsAbstractNodeTarget base;
    char **includes;
    char **sources;
    char **defines;
} SbsNodeCompile;

typedef struct SbsSectionCompile {
    SbsAbstractSectionTarget base;
    SbsNodeCompile **entries;
} SbsSectionCompile;

void sbs_section_compile_body_parse(SbsParser *parser, SbsSectionCompile *target_section, SbsNodeCompile *target);
SbsAbstractSectionTarget* sbs_section_compile_parse(SbsParser *parser);
SbsSectionCompile* sbs_section_compile_new(const struct FlSlice *name);
SbsNodeCompile* sbs_section_compile_add_node(SbsSectionCompile *section);
void sbs_node_compile_free(SbsNodeCompile *node);
void sbs_section_compile_free(SbsSectionCompile *section);

#endif /* SBS_LANG_TARGET_COMPILE_H */
