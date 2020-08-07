#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-executable.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/action.h"
#include "../lang/command.h"
#include "../lang/target.h"

SbsTargetExecutable* sbs_target_executable_new(const char *name)
{
    SbsTargetExecutable *executable_target = fl_malloc(sizeof(SbsTargetExecutable));

    executable_target->base.type = SBS_TARGET_EXECUTABLE;
    executable_target->base.name = fl_cstring_dup(name);

    return executable_target;
}

static void free_library(void *obj)
{
    if (!obj)
        return;

    SbsTargetLibrary *lib = (SbsTargetLibrary*)obj;

    if (lib->name)
        fl_cstring_free(lib->name);

    if (lib->path)
        fl_cstring_free(lib->path);
}

void sbs_target_executable_free(SbsTargetExecutable *target)
{
    sbs_target_base_free_members((SbsTarget*) target);

    if (target->objects)
        fl_array_free_each_pointer(target->objects, (FlArrayFreeElementFunc) sbs_source_free);

    if (target->libraries)
        fl_array_free_each(target->libraries, free_library);

    if (target->output_name)
        fl_cstring_free(target->output_name);

    if (target->defines)
        fl_array_free_each_pointer(target->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(target);
}

static void convert_library_node_to_library(SbsPropertyLibrary *dest, const SbsPropertyLibrary *src_obj)
{
    if (!dest)
        return;

    if (!src_obj)
    {
        memset(dest, 0, sizeof(SbsPropertyLibrary));
        return;
    }

    SbsTargetLibrary copy = {
        .name = src_obj->name ? fl_cstring_dup(src_obj->name) : NULL,
        .path = src_obj->path ? fl_cstring_dup(src_obj->path) : NULL
    };

    memcpy(dest, &copy, sizeof(SbsPropertyLibrary));
}

SbsTargetExecutable* sbs_target_executable_resolve(SbsResolveContext *context, const SbsSectionExecutable *executable_section)
{
    SbsTargetExecutable *executable_target = sbs_target_executable_new(executable_section->base.name);

    for (size_t i = 0; i < fl_array_length(executable_section->entries); i++)
    {
        const SbsNodeExecutable *executable_entry = executable_section->entries[i];

        if (executable_entry->base.condition && !sbs_eval_bool_expression(context->evalctx, executable_entry->base.condition))
            continue;

        sbs_target_base_resolve_members((SbsTarget*) executable_target, (const SbsAbstractNodeTarget*) executable_entry);

        executable_target->output_name = sbs_cstring_set(executable_target->output_name, executable_entry->output_name);
        executable_target->objects = sbs_source_array_extend(executable_target->objects, executable_entry->objects);
        executable_target->libraries = sbs_array_extend_copy_element(executable_target->libraries, executable_entry->libraries, (SbsArrayCopyElementFn) convert_library_node_to_library);
        executable_target->defines = sbs_cstring_array_extend(executable_target->defines, executable_entry->defines);
    }

    return executable_target;
}
