#ifndef SBS_LANG_CONFIGURATION_H
#define SBS_LANG_CONFIGURATION_H

#include <fllib.h>
#include "for.h"

typedef struct SbsNodeConfigCompile {
    const char *extension;
    char **flags;
} SbsNodeConfigCompile;

typedef struct SbsNodeConfigArchive {
    const char *extension;
    char **flags;
} SbsNodeConfigArchive;

typedef struct SbsNodeConfigShared {
    const char *extension;
    char **flags;
} SbsNodeConfigShared;

typedef struct SbsNodeConfigExecutable {
    const char *extension;
    char **flags;
} SbsNodeConfigExecutable;

typedef struct SbsNodeConfig {
    SbsNodeConfigCompile compile;
    SbsNodeConfigArchive archive;
    SbsNodeConfigShared shared;
    SbsNodeConfigExecutable executable;
    SbsSectionFor *for_clause;
} SbsNodeConfig;

typedef struct SbsSectionConfig {
    const char *name;
    char **extends;
    SbsSectionFor *for_clause;
    SbsNodeConfig **entries;
} SbsSectionConfig;

void sbs_section_config_free(SbsSectionConfig *configuration);

#endif /* SBS_LANG_CONFIGURATION_H */
