#ifndef SBS_LANG_PRESET_H
#define SBS_LANG_PRESET_H

#include "action.h"

typedef struct SbsSectionPreset {
    const char *name;
    char **envs;
    const char *toolchain;
    const char *config;
    char **targets;
    SbsPropertyActions actions;
} SbsSectionPreset;

SbsSectionPreset* sbs_section_preset_new(const struct FlSlice *name);
void sbs_section_preset_free(SbsSectionPreset *preset);

#endif /* SBS_LANG_PRESET_H */
