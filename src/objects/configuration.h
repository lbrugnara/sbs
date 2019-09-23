#ifndef SBS_CONFIGURATION_H
#define SBS_CONFIGURATION_H

#include <fllib.h>
#include "action.h"

struct SbsConfigCompile {
    const char *extension;
    const char *include_dir_flag;
    const char *define_flag;
    char **flags;
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

struct SbsConfiguration* sbs_config_parse(struct SbsParser *parser);
void sbs_config_free(struct SbsConfiguration *config);
void sbs_config_map_init(FlHashtable *config_map);
bool sbs_config_inheritance_resolve(struct SbsConfiguration *extended_config, const char *config_name, const FlHashtable config_map);
void sbs_config_inheritance_clean(struct SbsConfiguration *extended_config);

#endif /* SBS_CONFIGURATION_H */
