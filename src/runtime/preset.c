#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "preset.h"
#include "../lang/preset.h"

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

    if (preset->toolchains)
        fl_array_free_each_pointer(preset->toolchains, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->configs)
        fl_array_free_each_pointer(preset->configs, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->targets)
        fl_array_free_each_pointer(preset->targets, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->actions.before)
        fl_array_free_each_pointer(preset->actions.before, (FlArrayFreeElementFunc) sbs_command_free);

    if (preset->actions.after)
        fl_array_free_each_pointer(preset->actions.after, (FlArrayFreeElementFunc) sbs_command_free);

    fl_free(preset);
}

SbsPreset* sbs_preset_resolve(SbsResolveContext *context, const char *preset_name)
{
    SbsSectionPreset *preset_section = fl_hashtable_get(context->file->presets, preset_name);

    if (!preset_section)
        return NULL;

    SbsPreset *preset_object = sbs_preset_new(preset_section->name);

    preset_object->envs = sbs_cstring_array_extend(preset_object->envs, preset_section->envs);
    preset_object->toolchains = sbs_cstring_array_extend(preset_object->toolchains, preset_section->toolchains);
    preset_object->configs = sbs_cstring_array_extend(preset_object->configs, preset_section->configs);
    preset_object->targets = sbs_cstring_array_extend(preset_object->targets, preset_section->targets);
    
    preset_object->actions.before = sbs_command_array_extend_from_value_command(preset_object->actions.before, preset_section->actions.before);
    preset_object->actions.after = sbs_command_array_extend_from_value_command(preset_object->actions.after, preset_section->actions.after);

    return preset_object;
}
