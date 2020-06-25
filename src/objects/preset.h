#ifndef SBS_OBJECT_PRESET_H
#define SBS_OBJECT_PRESET_H

#include "action.h"

typedef struct {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    SbsActions actions;
} SbsPreset;

SbsPreset* sbs_preset_resolve(const SbsFile *file, const char *preset_name, const char *env_name);
void sbs_preset_free(SbsPreset *preset);

#endif /* SBS_OBJECT_PRESET_H */
