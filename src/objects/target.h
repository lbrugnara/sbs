#ifndef SBS_OBJECT_TARGET_H
#define SBS_OBJECT_TARGET_H

#include <fllib.h>

#include "../context.h"
#include "action.h"
#include "../parser/target.h"
#include "../parser/file.h"

#define SBS_TARGET_BASE_DEF()       \
    SbsSectionTargetType type;      \
    char *name;                     \
    char *output_dir;               \
    SbsActions actions

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
    SbsStringOrId *objects;
} SbsTargetArchive;

typedef struct SbsTargetShared {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsStringOrId *objects;
} SbsTargetShared;

typedef struct SbsTargetLibrary {
    char *path;
    char *name;
} SbsTargetLibrary;

typedef struct SbsTargetExecutable {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsTargetLibrary *libraries;
    SbsStringOrId *objects;
    char **defines;
} SbsTargetExecutable;

SbsTarget* sbs_target_resolve(SbsContext *context, const char *target_name, const SbsTarget *parent);
void sbs_target_free(SbsTarget *target);

#endif /* SBS_OBJECT_TARGET_H */
