#ifndef SBS_OBJECT_CONFIGURATION_H
#define SBS_OBJECT_CONFIGURATION_H

#include <fllib.h>
#include "../parser/file.h"

typedef struct {
    char *extension;
    char **flags;
} SbsConfigCompile;

typedef struct {
    char *extension;
    char **flags;
} SbsConfigArchive;

typedef struct {
    char *extension;
    char **flags;
} SbsConfigShared;

typedef struct {
    char *extension;
    char **flags;
} SbsConfigExecutable;

typedef struct {
    char *name;
    SbsConfigCompile compile;
    SbsConfigArchive archive;
    SbsConfigShared shared;
    SbsConfigExecutable executable;
} SbsConfiguration;

SbsConfiguration* sbs_config_resolve(const SbsFile *file, const char *config_name, const char *env_name);
void sbs_config_free(SbsConfiguration *config);

#endif /* SBS_OBJECT_CONFIGURATION_H */
