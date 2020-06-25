#ifndef SBS_PARSER_CONFIGURATION_H
#define SBS_PARSER_CONFIGURATION_H

#include <fllib.h>
#include "parser.h"

typedef struct {
    const char *extension;
    char **flags;
} SbsConfigCompileNode;

typedef struct {
    const char *extension;
    char **flags;
} SbsConfigArchiveNode;

typedef struct {
    const char *extension;
    char **flags;
} SbsConfigSharedNode;

typedef struct {
    const char *extension;
    char **flags;
} SbsConfigExecutableNode;

struct SbsConfigNode {
    SbsConfigCompileNode compile;
    SbsConfigArchiveNode archive;
    SbsConfigSharedNode shared;
    SbsConfigExecutableNode executable;
};

typedef struct {
    const char *name;
    char **extends;
    /* FlHashtable<string, struct SbsConfigNode> */
    FlHashtable *nodes;
} SbsConfigSection;

SbsConfigSection* sbs_config_section_parse(SbsParser *parser);
void sbs_config_section_free(SbsConfigSection *config);

#endif /* SBS_PARSER_CONFIGURATION_H */
