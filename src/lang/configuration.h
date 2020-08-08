#ifndef SBS_LANG_CONFIGURATION_H
#define SBS_LANG_CONFIGURATION_H

#include <fllib.h>
#include "expr.h"
#include "string.h"
#include "parser.h"

typedef struct SbsNodeConfigCompile {
    SbsString *extension;
    SbsString **flags;
} SbsNodeConfigCompile;

typedef struct SbsNodeConfigArchive {
    SbsString *extension;
    SbsString **flags;
} SbsNodeConfigArchive;

typedef struct SbsNodeConfigShared {
    SbsString *extension;
    SbsString **flags;
} SbsNodeConfigShared;

typedef struct SbsNodeConfigExecutable {
    SbsString *extension;
    SbsString **flags;
} SbsNodeConfigExecutable;

typedef struct SbsNodeConfig {
    SbsNodeConfigCompile compile;
    SbsNodeConfigArchive archive;
    SbsNodeConfigShared shared;
    SbsNodeConfigExecutable executable;
    SbsExpression *condition;
} SbsNodeConfig;

typedef struct SbsSectionConfig {
    const char *name;
    char **extends;
    SbsExpression *condition;
    SbsNodeConfig **entries;
} SbsSectionConfig;

SbsSectionConfig* sbs_section_config_parse(SbsParser *parser);
SbsSectionConfig* sbs_section_config_new(const struct FlSlice *name);
SbsNodeConfig* sbs_section_config_add_node(SbsSectionConfig *config_section);
void sbs_section_config_free(SbsSectionConfig *configuration);

#endif /* SBS_LANG_CONFIGURATION_H */
