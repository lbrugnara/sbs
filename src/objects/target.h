#ifndef SBS_OBJECT_TARGET_H
#define SBS_OBJECT_TARGET_H

#include <fllib.h>

#include "action.h"
#include "../parser/target.h"
#include "../parser/file.h"

#define SBS_TARGET_BASE_DEF()       \
    enum SbsTargetType type;        \
    char *name;                     \
    struct SbsActions actions;      \
    char *output_dir

struct SbsTarget {
    SBS_TARGET_BASE_DEF();
};

struct SbsTargetCompile {
    SBS_TARGET_BASE_DEF();
    char **includes;
    char **sources;
    char **defines;
};

struct SbsTargetArchive {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetShared {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetLibrary {
    char *path;
    char *name;
};

struct SbsTargetExecutable {
    SBS_TARGET_BASE_DEF();
    char *output_name;
    struct SbsTargetLibrary *libraries;
    struct SbsStringOrId *objects;
    char **defines;
};

struct SbsTarget* sbs_target_resolve(const struct SbsFile *file, const char *target_name, const char *env_name, const struct SbsTarget *caller);
void sbs_target_free(struct SbsTarget *target);

#endif /* SBS_OBJECT_TARGET_H */
