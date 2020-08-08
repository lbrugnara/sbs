#ifndef SBS_LANG_PRESET_H
#define SBS_LANG_PRESET_H

#include "parser.h"
#include "action.h"

typedef struct SbsSectionPreset {
    const char *name;
    char **envs;
    char **toolchains;
    char **configs;
    char **targets;
    SbsPropertyActions actions;
} SbsSectionPreset;

SbsSectionPreset* sbs_section_preset_parse(SbsParser *parser);
SbsSectionPreset* sbs_section_preset_new(const struct FlSlice *name);
void sbs_section_preset_free(SbsSectionPreset *preset);

#endif /* SBS_LANG_PRESET_H */
