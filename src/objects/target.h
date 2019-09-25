#ifndef SBS_OBJECT_TARGET_H
#define SBS_OBJECT_TARGET_H

#include <fllib.h>
#include "common.h"
#include "action.h"

enum SbsTargetType {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
};

struct SbsTargetSection {
    enum SbsTargetType type;
    const char *name;
    FlHashtable entries;
};

struct SbsTargetNode {
    struct SbsActionsNode actions;
    const char *output_dir;
};

struct SbsTargetCompileNode {
    struct SbsTargetNode base;
    char **includes;
    char **sources;
    char **defines;
};

struct SbsTargetArchiveNode {
    struct SbsTargetNode base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetSharedNode {
    struct SbsTargetNode base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetExecutableNode {
    struct SbsTargetNode base;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTarget {
    enum SbsTargetType type;
    const char *name;
    struct SbsActionsNode actions;
    const char *output_dir;
};

struct SbsTargetCompile {
    enum SbsTargetType type;
    const char *name;
    struct SbsActionsNode actions;
    const char *output_dir;
    char **includes;
    char **sources;
    char **defines;
};

struct SbsTargetArchive {
    enum SbsTargetType type;
    const char *name;
    struct SbsActionsNode actions;
    const char *output_dir;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetShared {
    enum SbsTargetType type;
    const char *name;
    struct SbsActionsNode actions;
    const char *output_dir;
    const char *output_name;
    struct SbsStringOrId *objects;
};

struct SbsTargetExecutable {
    enum SbsTargetType type;
    const char *name;
    struct SbsActionsNode actions;
    const char *output_dir;
    const char *output_name;
    struct SbsStringOrId *objects;
};

void sbs_target_free(struct SbsTargetSection *target);
struct SbsTargetSection* sbs_target_parse(struct SbsParser *parser);

struct SbsTarget* sbs_target_resolve(const char *target_name, FlHashtable target_map, const char *env_name);
void sbs_target_release(struct SbsTarget *target);

#endif /* SBS_OBJECT_TARGET_H */
