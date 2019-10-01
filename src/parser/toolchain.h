#ifndef SBS_PARSER_TOOLCHAIN_H
#define SBS_PARSER_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"

struct SbsToolchainCompilerNode {
    const char *bin;
    const char *include_dir_flag;
    const char *define_flag;
};

struct SbsToolchainArchiverNode {
    const char *bin;
};

struct SbsToolchainLinkerNode {
    const char *bin;
    const char *lib_dir_flag;
    const char *lib_flag;
};

struct SbsToolchainNode {
    struct SbsToolchainCompilerNode compiler;
    struct SbsToolchainLinkerNode linker;
    struct SbsToolchainArchiverNode archiver;
};

struct SbsToolchainSection {
    const char *name;
    /* FlHashtable<string, SbsToolchainNode> */
    FlHashtable nodes;
};

struct SbsToolchainSection* sbs_toolchain_section_parse(struct SbsParser *parser);
void sbs_toolchain_section_free(struct SbsToolchainSection *toolchain);

#endif /* SBS_PARSER_TOOLCHAIN_H */
