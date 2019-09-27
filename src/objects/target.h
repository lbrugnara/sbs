#ifndef SBS_OBJECT_TARGET_H
#define SBS_OBJECT_TARGET_H

#include <fllib.h>

#include "action.h"
#include "../parser/target.h"
#include "../parser/file.h"

#define SBS_TARGET_BASE_DEF()     \
    enum SbsTargetType type;    \
    char *name;                 \
    struct SbsActions actions

struct SbsTarget {
    SBS_TARGET_BASE_DEF();
    char *output_dir;
};

struct SbsTargetCompile {
    SBS_TARGET_BASE_DEF();
    char *output_dir;
    char **includes;
    char **sources;
    char **defines;
};

struct SbsTargetArchive {
    SBS_TARGET_BASE_DEF();
    char *output_dir;
    char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetShared {
    SBS_TARGET_BASE_DEF();
    char *output_dir;
    char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetExecutable {
    SBS_TARGET_BASE_DEF();
    char *output_dir;
    char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTarget* sbs_target_resolve(const struct SbsFile *file, const char *target_name, const char *env_name);
void sbs_target_free(struct SbsTarget *target);

#endif /* SBS_OBJECT_TARGET_H */
