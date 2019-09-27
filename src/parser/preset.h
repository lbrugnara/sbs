#ifndef SBS_PARSER_PRESET_H
#define SBS_PARSER_PRESET_H

#include "action.h"

struct SbsPresetSection {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    struct SbsActionsNode actions;
};

struct SbsPresetSection* sbs_preset_section_parse(struct SbsParser *parser);
void sbs_preset_section_free(struct SbsPresetSection *preset);

#endif /* SBS_PARSER_PRESET_H */
