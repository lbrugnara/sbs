#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "preset.h"

void sbs_preset_free(SbsPreset *preset)
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

    if (preset->actions.before)
        fl_array_free_each(preset->actions.before, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (preset->actions.after)
        fl_array_free_each(preset->actions.after, (FlArrayFreeElementFunc) sbs_value_command_free);

    fl_free(preset);
}
