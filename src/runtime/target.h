#ifndef SBS_RUNTIME_TARGET_H
#define SBS_RUNTIME_TARGET_H

#include <fllib.h>

#include "../lang/action.h"
#include "../lang/target.h"
#include "../lang/file.h"

#define SBS_TARGET_BASE_DEF()       \
    SbsSectionTargetType type;      \
    char *name;                     \
    char *output_dir;               \
    SbsPropertyActions actions

typedef struct SbsTarget {
    SBS_TARGET_BASE_DEF();
} SbsTarget;

typedef struct SbsTargetCompile {
    SBS_TARGET_BASE_DEF();
    char **includes;
    char **sources;
    char **defines;
} SbsTargetCompile;

typedef struct SbsTargetArchive {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsValueSource *objects;
} SbsTargetArchive;

typedef struct SbsTargetShared {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsValueSource *objects;
} SbsTargetShared;

typedef struct SbsTargetLibrary {
    char *path;
    char *name;
} SbsTargetLibrary;

typedef struct SbsTargetExecutable {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsTargetLibrary *libraries;
    SbsValueSource *objects;
    char **defines;
} SbsTargetExecutable;

SbsTargetCompile* sbs_target_compile_new(const char *name);
SbsTargetArchive* sbs_target_archive_new(const char *name);
SbsTargetShared* sbs_target_shared_new(const char *name);
SbsTargetExecutable* sbs_target_executable_new(const char *name);
void sbs_target_free(SbsTarget *target);

#endif /* SBS_RUNTIME_TARGET_H */
