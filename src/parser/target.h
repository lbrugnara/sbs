#ifndef SBS_PARSER_TARGET_H
#define SBS_PARSER_TARGET_H

#include <fllib.h>
#include "common.h"
#include "action.h"

typedef enum {
    SBS_TARGET_COMPILE,
    SBS_TARGET_ARCHIVE,
    SBS_TARGET_SHARED,
    SBS_TARGET_EXECUTABLE,
} SbsTargetType;

typedef struct {
    SbsTargetType type;
    const char *name;
    FlHashtable *entries;
} SbsTargetSection;

typedef struct {
    SbsActionsNode actions;
    const char *output_dir;
} SbsTargetNode;

typedef struct {
    SbsTargetNode base;
    char **includes;
    char **sources;
    char **defines;
} SbsTargetCompileNode;

typedef struct {
    SbsTargetNode base;
    const char *output_name;
    SbsStringOrId *objects;
} SbsTargetArchiveNode;

typedef struct {
    SbsTargetNode base;
    const char *output_name;
    SbsStringOrId *objects;
} SbsTargetSharedNode;

typedef struct {
    char *path;
    char *name;
} SbsTargetLibraryNode;

typedef struct {
    SbsTargetNode base;
    const char *output_name;
    SbsTargetLibraryNode *libraries;
    SbsStringOrId *objects;
    char **defines;
} SbsTargetExecutableNode;

SbsTargetSection* sbs_target_section_parse(SbsParser *parser);
void sbs_target_section_free(SbsTargetSection *target);

#endif /* SBS_PARSER_TARGET_H */
