#include <fllib.h>
#include "target.h"
#include "../lang/command.h"
#include "../utils.h"
#include "../lang/target.h"
#include "../lang/action.h"

static void merge_base_target(SbsContext *context, SbsTarget *extend, const SbsSectionTarget *source);
static void merge_compile_target(SbsTargetCompile *extend, const SbsSectionCompile *source, const SbsTarget *parent);
static void merge_archive_target(SbsTargetArchive *extend, const SbsSectionArchive *source);
static void merge_shared_target(SbsTargetShared *extend, const SbsSectionShared *source);
static void merge_executable_target(SbsTargetExecutable *extend, const SbsSectionExecutable *source);

SbsTarget* sbs_target_resolve(SbsContext *context, const char *target_name, const SbsTarget *parent)
{    
    const SbsAbstractSectionTarget *abs_target_section = fl_hashtable_get(context->file->targets, target_name);

    if (!abs_target_section)
        return NULL;

    SbsTarget *target = NULL;
    switch (abs_target_section->type)
    {
        case SBS_TARGET_COMPILE:
            target = fl_malloc(sizeof(SbsTargetCompile));
            break;
        case SBS_TARGET_ARCHIVE:
            target = fl_malloc(sizeof(SbsTargetArchive));
            break;
        case SBS_TARGET_SHARED:
            target = fl_malloc(sizeof(SbsTargetShared));
            break;
        case SBS_TARGET_EXECUTABLE:
            target = fl_malloc(sizeof(SbsTargetExecutable));
            break;
        default:
            return NULL;
    }

    target->name = fl_cstring_dup(abs_target_section->name);
    target->type = abs_target_section->type;
    

    for(size_t i=0; i < fl_array_length(abs_target_section->entries); i++)
    {
        SbsSectionTarget *target_section = abs_target_section->entries[i];

        if (target_section->for_clause && !sbs_eval_run(target_section->for_clause->condition, context->symbols))
            continue;

        merge_base_target(context, target, target_section);

        switch (abs_target_section->type)
        {
            case SBS_TARGET_COMPILE:
                merge_compile_target((SbsTargetCompile*) target, (SbsSectionCompile*) target_section, parent);
                break;
            case SBS_TARGET_ARCHIVE:
                merge_archive_target((SbsTargetArchive*) target, (SbsSectionArchive*) target_section);
                break;
            case SBS_TARGET_SHARED:
                merge_shared_target((SbsTargetShared*) target, (SbsSectionShared*) target_section);
                break;
            case SBS_TARGET_EXECUTABLE:
                merge_executable_target((SbsTargetExecutable*) target, (SbsSectionExecutable*) target_section);
                break;
            default:
                return NULL;
        }
    }

    return target;
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

static void merge_base_target(SbsContext *context, SbsTarget *extend, const SbsSectionTarget *source)
{
    extend->output_dir = sbs_string_set(extend->output_dir, source->output_dir);

    extend->actions.before = sbs_value_command_array_extend(extend->actions.before, source->actions.before);
    extend->actions.after = sbs_value_command_array_extend(extend->actions.after, source->actions.after);
}

static void merge_compile_target(SbsTargetCompile *extend, const SbsSectionCompile *source, const SbsTarget *parent)
{
    extend->includes = sbs_string_array_extend(extend->includes, source->includes);
    extend->sources = sbs_string_array_extend(extend->sources, source->sources);
    extend->defines = sbs_string_array_extend(extend->defines, source->defines);

    if (parent == NULL)
        return;

    if (parent->type == SBS_TARGET_EXECUTABLE)
    {
        SbsTargetExecutable *executable = (SbsTargetExecutable*) parent;
        extend->defines = sbs_string_array_extend(extend->defines, executable->defines);
    }
}

static void merge_archive_target(SbsTargetArchive *extend, const SbsSectionArchive *source)
{
    extend->output_name = sbs_string_set(extend->output_name, source->output_name);
    extend->objects = sbs_value_source_array_extend(extend->objects, source->objects);
}

static void merge_shared_target(SbsTargetShared *extend, const SbsSectionShared *source)
{
    extend->output_name = sbs_string_set(extend->output_name, source->output_name);
    extend->objects = sbs_value_source_array_extend(extend->objects, source->objects);
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

static void merge_executable_target(SbsTargetExecutable *extend, const SbsSectionExecutable *source)
{
    extend->output_name = sbs_string_set(extend->output_name, source->output_name);
    extend->objects = sbs_value_source_array_extend(extend->objects, source->objects);
    extend->libraries = sbs_array_extend(extend->libraries, source->libraries, (SbsArrayCopyElementFn) convert_library_node_to_library);
    extend->defines = sbs_string_array_extend(extend->defines, source->defines);
}
