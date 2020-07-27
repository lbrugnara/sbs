#ifndef SBS_LANG_CONFIGURATION_H
#define SBS_LANG_CONFIGURATION_H

#include <fllib.h>
#include "conditional.h"

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
    SbsStmtConditional *condition;
} SbsNodeConfig;

typedef struct SbsSectionConfig {
    const char *name;
    char **extends;
    SbsStmtConditional *condition;
    SbsNodeConfig **entries;
} SbsSectionConfig;

SbsSectionConfig* sbs_section_config_new(const struct FlSlice *name);
SbsNodeConfig* sbs_section_config_add_node(SbsSectionConfig *config_section);
void sbs_section_config_free(SbsSectionConfig *configuration);

#endif /* SBS_LANG_CONFIGURATION_H */
