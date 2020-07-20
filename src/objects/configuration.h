#ifndef SBS_OBJECT_CONFIGURATION_H
#define SBS_OBJECT_CONFIGURATION_H

#include <fllib.h>
#include "../context.h"
#include "../parser/file.h"
#include "../parser/for.h"

typedef struct SbsConfigCompile {
    char *extension;
    char **flags;
} SbsConfigCompile;

typedef struct SbsConfigArchive {
    char *extension;
    char **flags;
} SbsConfigArchive;

typedef struct SbsConfigShared {
    char *extension;
    char **flags;
} SbsConfigShared;

typedef struct SbsConfigExecutable {
    char *extension;
    char **flags;
} SbsConfigExecutable;

typedef struct SbsConfiguration {
    char *name;
    SbsConfigCompile compile;
    SbsConfigArchive archive;
    SbsConfigShared shared;
    SbsConfigExecutable executable;
} SbsConfiguration;

SbsConfiguration* sbs_config_resolve(SbsContext *context, const char *config_name);
void sbs_config_free(SbsConfiguration *config);

#endif /* SBS_OBJECT_CONFIGURATION_H */
