#include "preset.h"
#include "../lang/preset.h"

SbsPreset* sbs_preset_resolve(SbsContext *context, const char *preset_name)
{
    SbsSectionPreset *preset_section = fl_hashtable_get(context->file->presets, preset_name);

    if (!preset_section)
        return NULL;

    SbsPreset *preset_object = fl_malloc(sizeof(SbsPreset));

    preset_object->name = fl_cstring_dup(preset_section->name);
    preset_object->env = sbs_string_array_extend(preset_object->env, preset_section->env);
    preset_object->toolchain = preset_section->toolchain ? fl_cstring_dup(preset_section->toolchain) : NULL;
    preset_object->config = preset_section->config ? fl_cstring_dup(preset_section->config) : NULL;
    preset_object->target = preset_section->target ? fl_cstring_dup(preset_section->target) : NULL;
    
    preset_object->actions.before = sbs_value_command_array_extend(preset_object->actions.before, preset_section->actions.before);
    preset_object->actions.after = sbs_value_command_array_extend(preset_object->actions.after, preset_section->actions.after);

    return preset_object;
}

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
