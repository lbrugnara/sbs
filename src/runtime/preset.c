#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "preset.h"

SbsPreset* sbs_preset_new(const char *name)
{
    SbsPreset *preset = fl_malloc(sizeof(SbsPreset));

    preset->name = fl_cstring_dup(name);

    return preset;
}

void sbs_preset_free(SbsPreset *preset)
{
    fl_cstring_free(preset->name);

    if (preset->envs)
        fl_array_free_each_pointer(preset->envs, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->toolchain)
        fl_cstring_free(preset->toolchain);

    if (preset->config)
        fl_cstring_free(preset->config);

    if (preset->targets)
        fl_array_free_each_pointer(preset->targets, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->actions.before)
        fl_array_free_each(preset->actions.before, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (preset->actions.after)
        fl_array_free_each(preset->actions.after, (FlArrayFreeElementFunc) sbs_value_command_free);

    fl_free(preset);
}
