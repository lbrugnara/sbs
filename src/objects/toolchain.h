#ifndef SBS_OBJECT_TOOLCHAIN_H
#define SBS_OBJECT_TOOLCHAIN_H

#include <fllib.h>
#include "../parser/file.h"

typedef struct {
    char *bin;
    char *include_dir_flag;
    char *define_flag;
} SbsToolchainCompiler;

typedef struct {
    char *bin;
    char *lib_dir_flag;
    char *lib_flag;
} SbsToolchainLinker;

typedef struct {
    char *bin;
} SbsToolchainArchiver;

typedef struct {
    char *name;
    SbsToolchainCompiler compiler;
    SbsToolchainLinker linker;
    SbsToolchainArchiver archiver;
} SbsToolchain;

SbsToolchain* sbs_toolchain_resolve(const SbsFile *file, const char *toolchain_name, const char *env_name);
void sbs_toolchain_free(SbsToolchain *toolchain);

#endif /* SBS_OBJECT_TOOLCHAIN_H */
