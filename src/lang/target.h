#ifndef SBS_LANG_TARGET_H
#define SBS_LANG_TARGET_H

#include <fllib.h>
#include "action.h"
#include "source.h"
#include "for.h"

typedef enum SbsSectionTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
} SbsSectionTargetType;

typedef struct SbsSectionTarget {
    SbsPropertyActions actions;
    const char *output_dir;
    SbsSectionFor *for_clause;
} SbsSectionTarget;

typedef struct SbsSectionCompile {
    SbsSectionTarget base;
    char **includes;
    char **sources;
    char **defines;
} SbsSectionCompile;

typedef struct SbsSectionArchive {
    SbsSectionTarget base;
    const char *output_name;
    SbsValueSource *objects;
} SbsSectionArchive;

typedef struct SbsSectionShared {
    SbsSectionTarget base;
    const char *output_name;
    SbsValueSource *objects;
} SbsSectionShared;

typedef struct SbsPropertyLibrary {
    char *path;
    char *name;
} SbsPropertyLibrary;

typedef struct SbsSectionExecutable {
    SbsSectionTarget base;
    const char *output_name;
    SbsPropertyLibrary *libraries;
    SbsValueSource *objects;
    char **defines;
} SbsSectionExecutable;

typedef struct SbsAbstractSectionTarget {
    const char *name;
    SbsSectionTarget **entries;
    SbsSectionTargetType type;
} SbsAbstractSectionTarget;

void sbs_section_target_free(SbsAbstractSectionTarget *target_section);

#endif /* SBS_LANG_TARGET_H */
