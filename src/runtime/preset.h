#ifndef SBS_RUNTIME_PRESET_H
#define SBS_RUNTIME_PRESET_H

#include "../lang/action.h"

typedef struct SbsPreset {
    char *name;
    char **env;
    const char *toolchain;
    const char *config;
    const char *target;
    SbsPropertyActions actions;
} SbsPreset;

SbsPreset* sbs_preset_new(const char *name);
void sbs_preset_free(SbsPreset *preset);

#endif /* SBS_RUNTIME_PRESET_H */
