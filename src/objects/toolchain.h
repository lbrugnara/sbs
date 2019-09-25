#ifndef SBS_OBJECT_TOOLCHAIN_H
#define SBS_OBJECT_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"

struct SbsToolchainEntry {
    const char *compiler;
    const char *linker;
    const char *archiver;
};

struct SbsToolchainSection {
    const char *name;
    /* FlHashtable<string, SbsToolchainEntry> */
    FlHashtable entries;
};

struct SbsToolchainSection* sbs_toolchain_parse(struct SbsParser *parser);
void sbs_toolchain_free(struct SbsToolchainSection *toolchain);
const char* sbs_toolchain_get_compiler(const struct SbsToolchainSection *toolchain, const struct SbsEnv *env);
const char* sbs_toolchain_get_archiver(const struct SbsToolchainSection *toolchain, const struct SbsEnv *env);

#endif /* SBS_OBJECT_TOOLCHAIN_H */
