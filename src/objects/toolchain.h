#ifndef SBS_OBJECT_TOOLCHAIN_H
#define SBS_OBJECT_TOOLCHAIN_H

#include <fllib.h>
#include "../parser/file.h"

struct SbsToolchain {
    char *name;
    char *compiler;
    char *linker;
    char *archiver;
};

struct SbsToolchain* sbs_toolchain_resolve(const struct SbsFile *file, const char *toolchain_name, const char *env_name);
void sbs_toolchain_free(struct SbsToolchain *toolchain);

#endif /* SBS_OBJECT_TOOLCHAIN_H */
