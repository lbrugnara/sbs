#ifndef SBS_RUNTIME_PRESET_H
#define SBS_RUNTIME_PRESET_H

#include "resolve.h"
#include "../lang/command.h"

typedef struct SbsPreset {
    char *name;
    char **envs;
    char **toolchains;
    char **configs;
    char **targets;
    struct {
        SbsCommand **before;
        SbsCommand **after;
    } actions;
} SbsPreset;

SbsPreset* sbs_preset_new(const char *name);
void sbs_preset_free(SbsPreset *preset);

SbsPreset* sbs_preset_resolve(SbsResolveContext *context, const char *preset_name);

#endif /* SBS_RUNTIME_PRESET_H */
