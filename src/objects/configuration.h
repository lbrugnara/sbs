#ifndef SBS_CONFIGURATION_H
#define SBS_CONFIGURATION_H

#include <fllib.h>
#include "action.h"

struct SbsConfigCompile {
    const char *extension;
    const char *include_dir_flag;
    char **flags;
    char **defines;
};

struct SbsConfigArchive {
    const char *extension;
    char **flags;
};

struct SbsConfigShared {
    const char *extension;
    char **flags;
};

struct SbsConfigExecutable {
    const char *extension;
    char **flags;
};

struct SbsConfiguration {
    const char *name;
    char **extends;
    char **for_toolchains;
    struct SbsConfigCompile compile;
    struct SbsConfigArchive archive;
    struct SbsConfigShared shared;
    struct SbsConfigExecutable executable;
};

void sbs_config_map_init(FlHashtable *config);
void sbs_config_free(struct SbsConfiguration *config);
struct SbsConfiguration* sbs_config_parse(struct SbsParser *parser);

#endif /* SBS_CONFIGURATION_H */
