#ifndef SBS_LANG_PRESET_H
#define SBS_LANG_PRESET_H

#include "action.h"

typedef struct SbsSectionPreset {
    const char *name;
    char **env;
    const char *toolchain;
    const char *config;
    const char *target;
    SbsPropertyActions actions;
} SbsSectionPreset;

void sbs_section_preset_free(SbsSectionPreset *preset);

#endif /* SBS_LANG_PRESET_H */
