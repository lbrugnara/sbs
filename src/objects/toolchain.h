#ifndef SBS_TOOLCHAIN_H
#define SBS_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"
#include "environment.h"

struct SbsToolchainInfo {
    const char *compiler;
    const char *linker;
    const char *archiver;
};

struct SbsToolchain {
    const char *name;
    /* FlHashtable<string, SbsToolchainInfo> */
    FlHashtable toolchains;
};

struct SbsToolchain* sbs_toolchain_parse(struct SbsParser *parser);
void sbs_toolchain_free(struct SbsToolchain *toolchain);
void sbs_toolchain_map_init(FlHashtable *toolchains);
const char* sbs_toolchain_get_compiler(const struct SbsToolchain *toolchain, const struct SbsEnv *env);
const char* sbs_toolchain_get_archiver(const struct SbsToolchain *toolchain, const struct SbsEnv *env);
//bool sbs_toolchain_inheritance_resolve(struct SbsToolchain *extended_toolchain, const char *toolchain_name, const FlHashtable toolchain_map);
//void sbs_toolchain_inheritance_clean(struct SbsToolchain *extended_toolchain);

#endif /* SBS_TOOLCHAIN_H */
