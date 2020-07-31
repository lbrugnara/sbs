#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/action.h"
#include "../lang/command.h"
#include "../lang/target.h"

SbsTargetCompile* sbs_target_compile_new(const char *name)
{
    SbsTargetCompile *compile_target = fl_malloc(sizeof(SbsTargetCompile));

    compile_target->type = SBS_TARGET_COMPILE;
    compile_target->name = fl_cstring_dup(name);

    return compile_target;
}

SbsTargetArchive* sbs_target_archive_new(const char *name)
{
    SbsTargetArchive *archive_target = fl_malloc(sizeof(SbsTargetArchive));

    archive_target->type = SBS_TARGET_ARCHIVE;
    archive_target->name = fl_cstring_dup(name);

    return archive_target;
}

SbsTargetShared* sbs_target_shared_new(const char *name)
{
    SbsTargetShared *shared_target = fl_malloc(sizeof(SbsTargetShared));

    shared_target->type = SBS_TARGET_SHARED;
    shared_target->name = fl_cstring_dup(name);

    return shared_target;
}

SbsTargetExecutable* sbs_target_executable_new(const char *name)
{
    SbsTargetExecutable *executable_target = fl_malloc(sizeof(SbsTargetExecutable));

    executable_target->type = SBS_TARGET_EXECUTABLE;
    executable_target->name = fl_cstring_dup(name);

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

void sbs_target_free(SbsTarget *target)
{
    if (target->name)
        fl_cstring_free(target->name);

    if (target->actions.before)
        fl_array_free_each(target->actions.before, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target->actions.after)
        fl_array_free_each(target->actions.after, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target->output_dir)
        fl_cstring_free(target->output_dir);

    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            SbsTargetCompile *compile = (SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_free_each_pointer(compile->sources, (FlArrayFreeElementFunc) fl_cstring_free);

            if (compile->includes)
                fl_array_free_each_pointer(compile->includes, (FlArrayFreeElementFunc) fl_cstring_free);

            if (compile->defines)
                fl_array_free_each_pointer(compile->defines, (FlArrayFreeElementFunc) fl_cstring_free);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            SbsTargetArchive *archive = (SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_free_each(archive->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (archive->output_name)
                fl_cstring_free(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            SbsTargetShared *shared = (SbsTargetShared*)target;

            if (shared->objects)
                fl_array_free_each(shared->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (shared->output_name)
                fl_cstring_free(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            SbsTargetExecutable *executable = (SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_free_each(executable->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (executable->libraries)
                fl_array_free_each(executable->libraries, free_library);

            if (executable->output_name)
                fl_cstring_free(executable->output_name);

            if (executable->defines)
                fl_array_free_each_pointer(executable->defines, (FlArrayFreeElementFunc) fl_cstring_free);

            break;
        }
    }

    fl_free(target);
}

static void resolve_base_node_target(SbsTarget *extend, const SbsNodeTarget *source)
{
    extend->output_dir = sbs_cstring_set(extend->output_dir, source->output_dir);

    extend->actions.before = sbs_value_command_array_extend(extend->actions.before, source->actions.before);
    extend->actions.after = sbs_value_command_array_extend(extend->actions.after, source->actions.after);
}

static SbsTargetCompile* resolve_compile_target(SbsResolveContext *context, const SbsSectionCompile *compile_section, const SbsTarget *parent)
{
    SbsTargetCompile *compile_target = sbs_target_compile_new(compile_section->base.name);

    for (size_t i = 0; i < fl_array_length(compile_section->entries); i++)
    {
        const SbsNodeCompile *compile_entry = compile_section->entries[i];

        if (compile_entry->base.condition && !sbs_expression_eval_bool(context->symbols, compile_entry->base.condition->expr))
            continue;

        resolve_base_node_target((SbsTarget*) compile_target, (const SbsNodeTarget*) compile_entry);

        compile_target->includes = sbs_cstring_array_extend(compile_target->includes, compile_entry->includes);
        compile_target->sources = sbs_cstring_array_extend(compile_target->sources, compile_entry->sources);
        compile_target->defines = sbs_cstring_array_extend(compile_target->defines, compile_entry->defines);

        if (parent == NULL)
            continue;

        if (parent->type == SBS_TARGET_EXECUTABLE)
        {
            SbsTargetExecutable *executable = (SbsTargetExecutable*) parent;
            compile_target->defines = sbs_cstring_array_extend(compile_target->defines, executable->defines);
        }
    }

    return compile_target;
}

static SbsTargetArchive* resolve_archive_target(SbsResolveContext *context, const SbsSectionArchive *archive_section)
{
    SbsTargetArchive *archive_target = sbs_target_archive_new(archive_section->base.name);

    for (size_t i = 0; i < fl_array_length(archive_section->entries); i++)
    {
        const SbsNodeArchive *archive_entry = archive_section->entries[i];

        if (archive_entry->base.condition && !sbs_expression_eval_bool(context->symbols, archive_entry->base.condition->expr))
            continue;

        resolve_base_node_target((SbsTarget*) archive_target, (const SbsNodeTarget*) archive_entry);

        archive_target->output_name = sbs_cstring_set(archive_target->output_name, archive_entry->output_name);
        archive_target->objects = sbs_value_source_array_extend(archive_target->objects, archive_entry->objects);
    }

    return archive_target;
}

static SbsTargetShared* resolve_shared_target(SbsResolveContext *context, const SbsSectionShared *shared_section)
{
    SbsTargetShared *shared_target = sbs_target_shared_new(shared_section->base.name);

    for (size_t i = 0; i < fl_array_length(shared_section->entries); i++)
    {
        const SbsNodeShared *shared_entry = shared_section->entries[i];

        if (shared_entry->base.condition && !sbs_expression_eval_bool(context->symbols, shared_entry->base.condition->expr))
            continue;

        resolve_base_node_target((SbsTarget*) shared_target, (const SbsNodeTarget*) shared_entry);

        shared_target->output_name = sbs_cstring_set(shared_target->output_name, shared_entry->output_name);
        shared_target->objects = sbs_value_source_array_extend(shared_target->objects, shared_entry->objects);
    }

    return shared_target;
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

static SbsTargetExecutable* resolve_executable_target(SbsResolveContext *context, const SbsSectionExecutable *executable_section)
{
    SbsTargetExecutable *executable_target = sbs_target_executable_new(executable_section->base.name);

    for (size_t i = 0; i < fl_array_length(executable_section->entries); i++)
    {
        const SbsNodeExecutable *executable_entry = executable_section->entries[i];

        if (executable_entry->base.condition && !sbs_expression_eval_bool(context->symbols, executable_entry->base.condition->expr))
            continue;

        resolve_base_node_target((SbsTarget*) executable_target, (const SbsNodeTarget*) executable_entry);

        executable_target->output_name = sbs_cstring_set(executable_target->output_name, executable_entry->output_name);
        executable_target->objects = sbs_value_source_array_extend(executable_target->objects, executable_entry->objects);
        executable_target->libraries = sbs_array_extend(executable_target->libraries, executable_entry->libraries, (SbsArrayCopyElementFn) convert_library_node_to_library);
        executable_target->defines = sbs_cstring_array_extend(executable_target->defines, executable_entry->defines);
    }

    return executable_target;
}

SbsTarget* sbs_target_resolve(SbsResolveContext *context, const char *target_name, const SbsTarget *parent)
{    
    const SbsAbstractSectionTarget *abs_target_section = fl_hashtable_get(context->file->targets, target_name);

    if (!abs_target_section)
        return NULL;

    switch (abs_target_section->type)
    {
        case SBS_TARGET_COMPILE:
            return (SbsTarget*) resolve_compile_target(context, (SbsSectionCompile*) abs_target_section, parent);

        case SBS_TARGET_ARCHIVE:
            return (SbsTarget*) resolve_archive_target(context, (SbsSectionArchive*) abs_target_section);

        case SBS_TARGET_SHARED:
            return (SbsTarget*) resolve_shared_target(context, (SbsSectionShared*) abs_target_section);

        case SBS_TARGET_EXECUTABLE:
            return (SbsTarget*) resolve_executable_target(context, (SbsSectionExecutable*) abs_target_section);

        default:
            break;
    }

    return NULL;
}
