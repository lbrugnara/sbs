#ifndef SBS_PARSER_TOOLCHAIN_H
#define SBS_PARSER_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"

typedef struct {
    const char *bin;
    const char *include_dir_flag;
    const char *define_flag;
} SbsToolchainCompilerNode;

typedef struct {
    const char *bin;
} SbsToolchainArchiverNode;

typedef struct {
    const char *bin;
    const char *lib_dir_flag;
    const char *lib_flag;
} SbsToolchainLinkerNode;

typedef struct {
    SbsToolchainCompilerNode compiler;
    SbsToolchainLinkerNode linker;
    SbsToolchainArchiverNode archiver;
} SbsToolchainNode;

typedef struct {
    const char *name;
    /* FlHashtable<string, SbsToolchainNode> */
    FlHashtable *nodes;
} SbsToolchainSection;

SbsToolchainSection* sbs_toolchain_section_parse(SbsParser *parser);
void sbs_toolchain_section_free(SbsToolchainSection *toolchain);

#endif /* SBS_PARSER_TOOLCHAIN_H */
