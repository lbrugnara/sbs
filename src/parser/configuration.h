#ifndef SBS_PARSER_CONFIGURATION_H
#define SBS_PARSER_CONFIGURATION_H

#include <fllib.h>
#include "parser.h"

struct SbsConfigCompileNode {
    const char *extension;
    char **flags;
};

struct SbsConfigArchiveNode {
    const char *extension;
    char **flags;
};

struct SbsConfigSharedNode {
    const char *extension;
    char **flags;
};

struct SbsConfigExecutableNode {
    const char *extension;
    char **flags;
};

struct SbsConfigNode {
    struct SbsConfigCompileNode compile;
    struct SbsConfigArchiveNode archive;
    struct SbsConfigSharedNode shared;
    struct SbsConfigExecutableNode executable;
};

struct SbsConfigSection {
    const char *name;
    char **extends;
    /* FlHashtable<string, struct SbsConfigNode> */
    FlHashtable nodes;
};

struct SbsConfigSection* sbs_config_section_parse(struct SbsParser *parser);
void sbs_config_section_free(struct SbsConfigSection *config);

#endif /* SBS_PARSER_CONFIGURATION_H */
