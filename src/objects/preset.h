#ifndef SBS_OBJECT_PRESET_H
#define SBS_OBJECT_PRESET_H

#include "action.h"

struct SbsPreset {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    struct SbsActions actions;
};

struct SbsPreset* sbs_preset_resolve(const struct SbsFile *file, const char *preset_name, const char *env_name);
void sbs_preset_free(struct SbsPreset *preset);

#endif /* SBS_OBJECT_PRESET_H */
