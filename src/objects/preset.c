#include "preset.h"
#include "../parser/preset.h"

SbsPreset* sbs_preset_resolve(const SbsFile *file, const char *preset_name, const char *env_name)
{
    SbsPresetSection *preset_section = fl_hashtable_get(file->presets, preset_name);

    if (!preset_section)
        return NULL;

    SbsPreset *preset_object = fl_malloc(sizeof(SbsPreset));

    preset_object->name = fl_cstring_dup(preset_section->name);
    preset_object->env = preset_section->env ? fl_cstring_dup(preset_section->env) : NULL;
    preset_object->toolchain = preset_section->toolchain ? fl_cstring_dup(preset_section->toolchain) : NULL;
    preset_object->config = preset_section->config ? fl_cstring_dup(preset_section->config) : NULL;
    preset_object->target = preset_section->target ? fl_cstring_dup(preset_section->target) : NULL;
    
    
    SbsAction **before_actions = sbs_action_resolve_all(file, preset_section->actions.before, preset_object->env ? preset_object->env : env_name);
    if (before_actions)
    {
        preset_object->actions.before = sbs_common_extend_array(preset_object->actions.before, before_actions);
        fl_array_free(before_actions);
    }

    SbsAction **after_actions = sbs_action_resolve_all(file, preset_section->actions.after, preset_object->env ? preset_object->env : env_name);
    if (after_actions)
    {
        preset_object->actions.after = sbs_common_extend_array(preset_object->actions.after, after_actions);
        fl_array_free(after_actions);
    }

    return preset_object;
}

void sbs_preset_free(SbsPreset *preset)
{
    fl_cstring_free(preset->name);
    if (preset->env)
        fl_cstring_free(preset->env);
    if (preset->toolchain)
        fl_cstring_free(preset->toolchain);
    if (preset->config)
        fl_cstring_free(preset->config);
    if (preset->target)
        fl_cstring_free(preset->target);

    if (preset->actions.before)
        sbs_action_free_all(preset->actions.before);

    if (preset->actions.after)
        sbs_action_free_all(preset->actions.after);

    fl_free(preset);
}
