#ifndef SBS_TOOLCHAIN_H
#define SBS_TOOLCHAIN_H

#include <fllib.h>
#include "parser.h"

struct SbsToolchain {
    const char *name;
    char **extends;
    char **for_envs;
    const char *compiler;
    const char *linker;
    const char *archiver;
};

void sbs_toolchain_map_init(FlHashtable *toolchains);
void sbs_toolchain_free(struct SbsToolchain *toolchain);
struct SbsToolchain* sbs_toolchain_parse(struct SbsParser *parser);

#endif /* SBS_TOOLCHAIN_H */
