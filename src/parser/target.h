#ifndef SBS_PARSER_TARGET_H
#define SBS_PARSER_TARGET_H

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

struct SbsTargetSection* sbs_target_section_parse(struct SbsParser *parser);
void sbs_target_section_free(struct SbsTargetSection *target);

#endif /* SBS_PARSER_TARGET_H */
