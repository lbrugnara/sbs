#ifndef SBS_OBJECT_CONFIGURATION_H
#define SBS_OBJECT_CONFIGURATION_H

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

struct SbsConfigEntry {
    struct SbsConfigCompile compile;
    struct SbsConfigArchive archive;
    struct SbsConfigShared shared;
    struct SbsConfigExecutable executable;
};

struct SbsConfigSection {
    const char *name;
    char **extends;
    /* FlHashtable<string, struct SbsConfigEntry> */
    FlHashtable entries;
};

struct SbsConfiguration {
    const char *name;
    struct SbsConfigCompile compile;
    struct SbsConfigArchive archive;
    struct SbsConfigShared shared;
    struct SbsConfigExecutable executable;
};

struct SbsConfigSection* sbs_config_parse(struct SbsParser *parser);
void sbs_config_free(struct SbsConfigSection *config);

struct SbsConfiguration* sbs_config_resolve(const char *config_name, FlHashtable config_map, const char *env_name);
void sbs_config_release(struct SbsConfiguration *config);

#endif /* SBS_OBJECT_CONFIGURATION_H */
