#ifndef SBS_TARGET_H
#define SBS_TARGET_H

#include "common.h"
#include "action.h"

enum SbsTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
};

struct SbsTarget {
    enum SbsTargetType type;
    const char *name;
    struct SbsActions actions;
    const char *output_dir;
};

struct SbsTargetCompile {
    struct SbsTarget base;
    char **includes;
    char **sources;
    char **defines;
};

struct SbsTargetArchive {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetShared {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetExecutable {
    struct SbsTarget base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

void sbs_target_map_init(FlHashtable *targets);
void sbs_target_free(struct SbsTarget *target);
struct SbsTarget* sbs_target_parse(struct SbsParser *parser);

#endif /* SBS_TARGET_H */
