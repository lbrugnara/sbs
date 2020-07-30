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

    preset_object->envs = sbs_cstring_array_extend(preset_object->envs, preset_section->envs);
    preset_object->toolchains = sbs_cstring_array_extend(preset_object->toolchains, preset_section->toolchains);
    preset_object->configs = sbs_cstring_array_extend(preset_object->configs, preset_section->configs);
    preset_object->targets = sbs_cstring_array_extend(preset_object->targets, preset_section->targets);
    
    preset_object->actions.before = sbs_value_command_array_extend(preset_object->actions.before, preset_section->actions.before);
    preset_object->actions.after = sbs_value_command_array_extend(preset_object->actions.after, preset_section->actions.after);

    return preset_object;
}
