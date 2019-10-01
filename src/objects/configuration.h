#ifndef SBS_OBJECT_CONFIGURATION_H
#define SBS_OBJECT_CONFIGURATION_H

#include <fllib.h>
#include "../parser/file.h"

struct SbsConfigCompile {
    char *extension;
    char *include_dir_flag;
    char *define_flag;
    char **flags;
};

struct SbsConfigArchive {
    char *extension;
    char **flags;
};

struct SbsConfigShared {
    char *extension;
    char **flags;
};

struct SbsConfigExecutable {
    char *extension;
    char *lib_dir_flag;
    char *lib_flag;
    char **flags;
};

struct SbsConfiguration {
    char *name;
    struct SbsConfigCompile compile;
    struct SbsConfigArchive archive;
    struct SbsConfigShared shared;
    struct SbsConfigExecutable executable;
};

struct SbsConfiguration* sbs_config_resolve(const struct SbsFile *file, const char *config_name, const char *env_name);
void sbs_config_free(struct SbsConfiguration *config);

#endif /* SBS_OBJECT_CONFIGURATION_H */
