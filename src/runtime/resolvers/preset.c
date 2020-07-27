#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "preset.h"
#include "../../lang/preset.h"

SbsPreset* sbs_preset_resolve(SbsContext *context, const char *preset_name)
{
    SbsSectionPreset *preset_section = fl_hashtable_get(context->file->presets, preset_name);

    if (!preset_section)
        return NULL;

    SbsPreset *preset_object = sbs_preset_new(preset_section->name);

    preset_object->env = sbs_string_array_extend(preset_object->env, preset_section->env);
    preset_object->toolchain = preset_section->toolchain ? fl_cstring_dup(preset_section->toolchain) : NULL;
    preset_object->config = preset_section->config ? fl_cstring_dup(preset_section->config) : NULL;
    preset_object->target = preset_section->target ? fl_cstring_dup(preset_section->target) : NULL;
    
    preset_object->actions.before = sbs_value_command_array_extend(preset_object->actions.before, preset_section->actions.before);
    preset_object->actions.after = sbs_value_command_array_extend(preset_object->actions.after, preset_section->actions.after);

    return preset_object;
}
