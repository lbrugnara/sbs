#ifndef SBS_OBJECT_PRESET_H
#define SBS_OBJECT_PRESET_H

#include "action.h"

struct SbsPreset {
    const char *name;
    const char *env;
    const char *toolchain;
    const char *config;
    const char *target;
    struct SbsActionsNode actions;
};

void sbs_preset_free(struct SbsPreset *preset);
struct SbsPreset* sbs_preset_parse(struct SbsParser *parser);

#endif /* SBS_OBJECT_PRESET_H */
