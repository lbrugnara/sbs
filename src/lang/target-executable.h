#ifndef SBS_LANG_TARGET_EXECUTABLE_H
#define SBS_LANG_TARGET_EXECUTABLE_H

#include "target.h"
#include "parser.h"

typedef struct SbsPropertyLibrary {
    char *path;
    char *name;
} SbsPropertyLibrary;

typedef struct SbsNodeExecutable {
    SbsAbstractNodeTarget base;
    const char *output_name;
    SbsPropertyLibrary *libraries;
    SbsSource **objects;
    char **defines;
} SbsNodeExecutable;

typedef struct SbsSectionExecutable {
    SbsAbstractSectionTarget base;
    SbsNodeExecutable **entries;
} SbsSectionExecutable;

void sbs_section_executable_body_parse(SbsParser *parser, SbsSectionExecutable *target_section, SbsNodeExecutable *target);
SbsAbstractSectionTarget* sbs_section_executable_parse(SbsParser *parser);
SbsSectionExecutable* sbs_section_executable_new(const struct FlSlice *name);
SbsNodeExecutable* sbs_section_executable_add_node(SbsSectionExecutable *section);
void sbs_node_executable_free(SbsNodeExecutable *node);
void sbs_section_executable_free(SbsSectionExecutable *section);

#endif /* SBS_LANG_TARGET_EXECUTABLE_H */
