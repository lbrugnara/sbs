#ifndef SBS_PARSER_TOOLCHAIN_H
#define SBS_PARSER_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"
#include "for.h"

typedef struct SbsNodeToolchainCompiler {
    const char *bin;
    const char *include_dir_flag;
    const char *define_flag;
} SbsNodeToolchainCompiler;

typedef struct SbsNodeToolchainArchiver {
    const char *bin;
} SbsNodeToolchainArchiver;

typedef struct SbsNodeToolchainLinker {
    const char *bin;
    const char *lib_dir_flag;
    const char *lib_flag;
} SbsNodeToolchainLinker;

typedef struct SbsNodeToolchain {
    SbsNodeToolchainCompiler compiler;
    SbsNodeToolchainLinker linker;
    SbsNodeToolchainArchiver archiver;
    SbsSectionFor *for_clause;
} SbsNodeToolchain;

typedef struct SbsSectionToolchain {
    const char *name;
    SbsSectionFor *for_clause;
    SbsNodeToolchain **entries;
} SbsSectionToolchain;

SbsSectionToolchain* sbs_section_toolchain_parse(SbsParser *parser);
void sbs_section_toolchain_free(SbsSectionToolchain *toolchain);

#endif /* SBS_PARSER_TOOLCHAIN_H */
