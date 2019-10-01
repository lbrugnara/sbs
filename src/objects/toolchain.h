#ifndef SBS_OBJECT_TOOLCHAIN_H
#define SBS_OBJECT_TOOLCHAIN_H

#include <fllib.h>
#include "../parser/file.h"

struct SbsToolchainCompiler {
    char *bin;
    char *include_dir_flag;
    char *define_flag;
};

struct SbsToolchainLinker {
    char *bin;
    char *lib_dir_flag;
    char *lib_flag;
};

struct SbsToolchainArchiver {
    char *bin;
};

struct SbsToolchain {
    char *name;
    struct SbsToolchainCompiler compiler;
    struct SbsToolchainLinker linker;
    struct SbsToolchainArchiver archiver;
};

struct SbsToolchain* sbs_toolchain_resolve(const struct SbsFile *file, const char *toolchain_name, const char *env_name);
void sbs_toolchain_free(struct SbsToolchain *toolchain);

#endif /* SBS_OBJECT_TOOLCHAIN_H */
