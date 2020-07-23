#ifndef SBS_PARSER_CONFIGURATION_H
#define SBS_PARSER_CONFIGURATION_H

#include <fllib.h>
#include "parser.h"
#include "for.h"

typedef struct SbsNodeConfigCompile {
    const char *extension;
    char **flags;
} SbsNodeConfigCompile;

typedef struct SbsNodeConfigArchive {
    const char *extension;
    char **flags;
} SbsNodeConfigArchive;

typedef struct SbsNodeConfigShared {
    const char *extension;
    char **flags;
} SbsNodeConfigShared;

typedef struct SbsNodeConfigExecutable {
    const char *extension;
    char **flags;
} SbsNodeConfigExecutable;

typedef struct SbsNodeConfig {
    SbsNodeConfigCompile compile;
    SbsNodeConfigArchive archive;
    SbsNodeConfigShared shared;
    SbsNodeConfigExecutable executable;
    SbsSectionFor *for_clause;
} SbsNodeConfig;

typedef struct SbsSectionConfig {
    const char *name;
    char **extends;
    SbsSectionFor *for_clause;
    SbsNodeConfig **entries;
} SbsSectionConfig;

SbsSectionConfig* sbs_section_config_parse(SbsParser *parser);
void sbs_section_config_free(SbsSectionConfig *config);

#endif /* SBS_PARSER_CONFIGURATION_H */
