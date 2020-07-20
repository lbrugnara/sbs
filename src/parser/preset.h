#ifndef SBS_PARSER_PRESET_H
#define SBS_PARSER_PRESET_H

#include "action.h"

typedef struct SbsSectionPreset {
    const char *name;
    char **env;
    const char *toolchain;
    const char *config;
    const char *target;
    SbsPropertyActions actions;
} SbsSectionPreset;

SbsSectionPreset* sbs_section_preset_parse(SbsParser *parser);
void sbs_section_preset_free(SbsSectionPreset *preset);

#endif /* SBS_PARSER_PRESET_H */
