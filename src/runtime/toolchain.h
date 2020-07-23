#ifndef SBS_RUNTIME_TOOLCHAIN_H
#define SBS_RUNTIME_TOOLCHAIN_H

#include <fllib.h>
#include "context.h"
#include "../lang/file.h"
#include "../lang/for.h"

typedef struct SbsToolchainCompiler {
    char *bin;
    char *include_dir_flag;
    char *define_flag;
} SbsToolchainCompiler;

typedef struct SbsToolchainLinker {
    char *bin;
    char *lib_dir_flag;
    char *lib_flag;
} SbsToolchainLinker;

typedef struct SbsToolchainArchiver {
    char *bin;
} SbsToolchainArchiver;

typedef struct SbsToolchain {
    char *name;
    SbsToolchainCompiler compiler;
    SbsToolchainLinker linker;
    SbsToolchainArchiver archiver;
} SbsToolchain;

SbsToolchain* sbs_toolchain_resolve(SbsContext *context, const char *toolchain_name);
void sbs_toolchain_free(SbsToolchain *toolchain);

#endif /* SBS_RUNTIME_TOOLCHAIN_H */
