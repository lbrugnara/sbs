#ifndef SBS_RUNTIME_CONFIGURATION_H
#define SBS_RUNTIME_CONFIGURATION_H

#include "../lang/expr-string.h"
#include "resolve.h"

typedef struct SbsConfigCompile {
    SbsStringExpr *extension;
    SbsStringExpr **flags;
} SbsConfigCompile;

typedef struct SbsConfigArchive {
    SbsStringExpr *extension;
    SbsStringExpr **flags;
} SbsConfigArchive;

typedef struct SbsConfigShared {
    SbsStringExpr *extension;
    SbsStringExpr **flags;
} SbsConfigShared;

typedef struct SbsConfigExecutable {
    SbsStringExpr *extension;
    SbsStringExpr **flags;
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
SbsConfiguration* sbs_config_resolve(SbsResolveContext *context, const char *config_name);

#endif /* SBS_RUNTIME_CONFIGURATION_H */
