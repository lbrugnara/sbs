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

struct SbsConfigSection* sbs_config_parse(struct SbsParser *parser);
void sbs_config_free(struct SbsConfigSection *config);
void sbs_config_map_init(FlHashtable *config_map);

struct SbsConfigSection* sbs_config_resolve(FlHashtable config_map, const char *config_name, const char *env_name);
void sbs_config_delete(struct SbsConfigSection *config);

struct SbsConfigArchive* sbs_config_archive_get(const struct SbsConfigSection *config, const char *env_name);
void sbs_config_archive_free(struct SbsConfigArchive *archive);
struct SbsConfigCompile* sbs_config_compile_get(const struct SbsConfigSection *config, const char *env_name);
void sbs_config_compile_free(struct SbsConfigCompile *compile);

#endif /* SBS_CONFIGURATION_H */
