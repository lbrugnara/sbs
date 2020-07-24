#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "preset.h"

void sbs_section_preset_free(SbsSectionPreset *preset)
{
    fl_cstring_free(preset->name);

    if (preset->env)
        fl_array_free_each_pointer(preset->env, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->toolchain)
        fl_cstring_free(preset->toolchain);

    if (preset->config)
        fl_cstring_free(preset->config);

    if (preset->target)
        fl_cstring_free(preset->target);

    sbs_property_actions_free(&preset->actions);

    fl_free(preset);
}