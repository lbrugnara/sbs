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

typedef struct SbsNodeTarget {
    SbsPropertyActions actions;
    const char *output_dir;
    SbsStmtConditional *condition;
} SbsNodeTarget;

typedef struct SbsNodeCompile {
    SbsNodeTarget base;
    char **includes;
    char **sources;
    char **defines;
} SbsNodeCompile;

typedef struct SbsNodeArchive {
    SbsNodeTarget base;
    const char *output_name;
    SbsValueSource *objects;
} SbsNodeArchive;

typedef struct SbsNodeShared {
    SbsNodeTarget base;
    const char *output_name;
    SbsValueSource *objects;
} SbsNodeShared;

typedef struct SbsPropertyLibrary {
    char *path;
    char *name;
} SbsPropertyLibrary;

typedef struct SbsNodeExecutable {
    SbsNodeTarget base;
    const char *output_name;
    SbsPropertyLibrary *libraries;
    SbsValueSource *objects;
    char **defines;
} SbsNodeExecutable;

typedef struct SbsAbstractSectionTarget {
    const char *name;
    SbsSectionTargetType type;
} SbsAbstractSectionTarget;

typedef struct SbsSectionCompile {
    SbsAbstractSectionTarget base;
    SbsNodeCompile **entries;
} SbsSectionCompile;

typedef struct SbsSectionArchive {
    SbsAbstractSectionTarget base;
    SbsNodeArchive **entries;
} SbsSectionArchive;

typedef struct SbsSectionShared {
    SbsAbstractSectionTarget base;
    SbsNodeShared **entries;
} SbsSectionShared;

typedef struct SbsSectionExecutable {
    SbsAbstractSectionTarget base;
    SbsNodeExecutable **entries;
} SbsSectionExecutable;

SbsSectionCompile* sbs_section_compile_new(const struct FlSlice *name);
SbsSectionArchive* sbs_section_archive_new(const struct FlSlice *name);
SbsSectionShared* sbs_section_shared_new(const struct FlSlice *name);
SbsSectionExecutable* sbs_section_executable_new(const struct FlSlice *name);

SbsNodeCompile* sbs_section_compile_add_node(SbsSectionCompile *target_section);
SbsNodeArchive* sbs_section_archive_add_node(SbsSectionArchive *target_section);
SbsNodeShared* sbs_section_shared_add_node(SbsSectionShared *target_section);
SbsNodeExecutable* sbs_section_executable_add_node(SbsSectionExecutable *target_section);

void sbs_section_target_free(SbsAbstractSectionTarget *target_section);

#endif /* SBS_LANG_TARGET_H */
