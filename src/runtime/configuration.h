#ifndef SBS_RUNTIME_CONFIGURATION_H
#define SBS_RUNTIME_CONFIGURATION_H

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

void sbs_config_free(SbsConfiguration *config);

#endif /* SBS_RUNTIME_CONFIGURATION_H */
