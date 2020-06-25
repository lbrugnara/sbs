#ifndef SBS_OBJECT_TARGET_H
#define SBS_OBJECT_TARGET_H

#include <fllib.h>

#include "action.h"
#include "../parser/target.h"
#include "../parser/file.h"

#define SBS_TARGET_BASE_DEF()       \
    SbsTargetType type;        \
    char *name;                     \
    SbsActions actions;      \
    char *output_dir

typedef struct {
    SBS_TARGET_BASE_DEF();
} SbsTarget;

typedef struct {
    SBS_TARGET_BASE_DEF();
    char **includes;
    char **sources;
    char **defines;
} SbsTargetCompile;

typedef struct {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsStringOrId *objects;
} SbsTargetArchive;

typedef struct {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsStringOrId *objects;
} SbsTargetShared;

typedef struct {
    char *path;
    char *name;
} SbsTargetLibrary;

typedef struct {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    SbsTargetLibrary *libraries;
    SbsStringOrId *objects;
    char **defines;
} SbsTargetExecutable;

SbsTarget* sbs_target_resolve(const SbsFile *file, const char *target_name, const char *env_name, const SbsTarget *caller);
void sbs_target_free(SbsTarget *target);

#endif /* SBS_OBJECT_TARGET_H */
