#ifndef SBS_PARSER_TOOLCHAIN_H
#define SBS_PARSER_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"

struct SbsToolchainNode {
    const char *compiler;
    const char *linker;
    const char *archiver;
};

struct SbsToolchainSection {
    const char *name;
    /* FlHashtable<string, SbsToolchainNode> */
    FlHashtable nodes;
};

struct SbsToolchainSection* sbs_toolchain_section_parse(struct SbsParser *parser);
void sbs_toolchain_section_free(struct SbsToolchainSection *toolchain);

#endif /* SBS_PARSER_TOOLCHAIN_H */
