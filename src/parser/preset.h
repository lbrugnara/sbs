#ifndef SBS_PARSER_PRESET_H
#define SBS_PARSER_PRESET_H

#include "action.h"

typedef struct {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    SbsActionsNode actions;
} SbsPresetSection;

SbsPresetSection* sbs_preset_section_parse(SbsParser *parser);
void sbs_preset_section_free(SbsPresetSection *preset);

#endif /* SBS_PARSER_PRESET_H */
