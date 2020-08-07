#ifndef SBS_LANG_TOOLCHAIN_H
#define SBS_LANG_TOOLCHAIN_H

#include <fllib.h>
#include "environment.h"
#include "../runtime/eval.h"

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
    SbsExpression *condition;
} SbsNodeToolchain;

typedef struct SbsSectionToolchain {
    const char *name;
    SbsExpression *condition;
    SbsNodeToolchain **entries;
} SbsSectionToolchain;

SbsSectionToolchain* sbs_section_toolchain_new(const struct FlSlice *name);
SbsNodeToolchain* sbs_section_toolchain_add_node(SbsSectionToolchain *toolchain_section);
void sbs_section_toolchain_free(SbsSectionToolchain *toolchain);

#endif /* SBS_LANG_TOOLCHAIN_H */
