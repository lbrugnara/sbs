#ifndef SBS_RUNTIME_CONFIGURATION_H
#define SBS_RUNTIME_CONFIGURATION_H

#include "string.h"

typedef struct SbsConfigCompile {
    SbsString *extension;
    SbsString **flags;
} SbsConfigCompile;

typedef struct SbsConfigArchive {
    SbsString *extension;
    SbsString **flags;
} SbsConfigArchive;

typedef struct SbsConfigShared {
    SbsString *extension;
    SbsString **flags;
} SbsConfigShared;

typedef struct SbsConfigExecutable {
    SbsString *extension;
    SbsString **flags;
} SbsConfigExecutable;

typedef struct SbsConfiguration {
    char *name;
    SbsConfigCompile compile;
    SbsConfigArchive archive;
    SbsConfigShared shared;
    SbsConfigExecutable executable;
} SbsConfiguration;

SbsConfiguration* sbs_config_new(const char *name);
void sbs_config_free(SbsConfiguration *config);

#endif /* SBS_RUNTIME_CONFIGURATION_H */
