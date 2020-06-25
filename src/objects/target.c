#include <fllib.h>
#include "target.h"
#include "../common/common.h"
#include "../parser/target.h"
#include "../parser/action.h"

static void merge_base_target(const SbsFile *file, const char *env_name, SbsTarget *extend, const SbsTargetNode *source);
static void merge_compile_target(SbsTargetCompile *extend, const SbsTargetCompileNode *source, const SbsTarget *parent);
static void merge_archive_target(SbsTargetArchive *extend, const SbsTargetArchiveNode *source);
static void merge_shared_target(SbsTargetShared *extend, const SbsTargetSharedNode *source);
static void merge_executable_target(SbsTargetExecutable *extend, const SbsTargetExecutableNode *source);

SbsTarget* sbs_target_resolve(const SbsFile *file, const char *target_name, const char *env_name, const SbsTarget *parent)
{    
    const SbsTargetSection *target_section = fl_hashtable_get(file->targets, target_name);

    if (!target_section)
        return NULL;

    SbsTarget *target_obj = NULL;
    switch (target_section->type)
    {
        case SBS_TARGET_COMPILE:
            target_obj = fl_malloc(sizeof(SbsTargetCompile));
            break;
        case SBS_TARGET_ARCHIVE:
            target_obj = fl_malloc(sizeof(SbsTargetArchive));
            break;
        case SBS_TARGET_SHARED:
            target_obj = fl_malloc(sizeof(SbsTargetShared));
            break;
        case SBS_TARGET_EXECUTABLE:
            target_obj = fl_malloc(sizeof(SbsTargetExecutable));
            break;
        default:
            return NULL;
    }

    target_obj->name = fl_cstring_dup(target_section->name);
    target_obj->type = target_section->type;

    FlList *hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(target_section->entries, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, env_name));

    fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, SBS_BASE_OBJECT_KEY));

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const SbsTargetNode *ancestor = (const SbsTargetNode*)node->value;
        
        merge_base_target(file, env_name, target_obj, ancestor);

        switch (target_section->type)
        {
            case SBS_TARGET_COMPILE:
                merge_compile_target((SbsTargetCompile*)target_obj, (SbsTargetCompileNode*)node->value, parent);
                break;
            case SBS_TARGET_ARCHIVE:
                merge_archive_target((SbsTargetArchive*)target_obj, (SbsTargetArchiveNode*)node->value);
                break;
            case SBS_TARGET_SHARED:
                merge_shared_target((SbsTargetShared*)target_obj, (SbsTargetSharedNode*)node->value);
                break;
            case SBS_TARGET_EXECUTABLE:
                merge_executable_target((SbsTargetExecutable*)target_obj, (SbsTargetExecutableNode*)node->value);
                break;
            default:
                return NULL;
        }

        node = node->next;
    }

    fl_list_free(hierarchy);

    return target_obj;
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

    //sbs_actions_node_free(&target->actions);
    if (target->actions.before)
        sbs_action_free_all(target->actions.before);

    if (target->actions.after)
        sbs_action_free_all(target->actions.after);

    if (target->output_dir)
        fl_cstring_free(target->output_dir);

    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            SbsTargetCompile *compile = (SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_free_each(compile->sources, sbs_common_free_string);

            if (compile->includes)
                fl_array_free_each(compile->includes, sbs_common_free_string);

            if (compile->defines)
                fl_array_free_each(compile->defines, sbs_common_free_string);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            SbsTargetArchive *archive = (SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_free_each(archive->objects, sbs_common_free_string_or_id);

            if (archive->output_name)
                fl_cstring_free(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            SbsTargetShared *shared = (SbsTargetShared*)target;

            if (shared->objects)
                fl_array_free_each(shared->objects, sbs_common_free_string_or_id);

            if (shared->output_name)
                fl_cstring_free(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            SbsTargetExecutable *executable = (SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_free_each(executable->objects, sbs_common_free_string_or_id);

            if (executable->libraries)
                fl_array_free_each(executable->libraries, free_library);

            if (executable->output_name)
                fl_cstring_free(executable->output_name);

            if (executable->defines)
                fl_array_free_each(executable->defines, sbs_common_free_string);

            break;
        }
    }

    fl_free(target);
}

static void merge_base_target(const SbsFile *file, const char *env_name, SbsTarget *extend, const SbsTargetNode *source)
{
    extend->output_dir = sbs_common_set_string(extend->output_dir, source->output_dir);

    SbsAction **before_actions = sbs_action_resolve_all(file, source->actions.before, env_name);
    if (before_actions)
    {
        extend->actions.before = sbs_common_extend_array(extend->actions.before, before_actions);
        fl_array_free(before_actions);
    }

    SbsAction **after_actions = sbs_action_resolve_all(file, source->actions.after, env_name);
    if (after_actions)
    {
        extend->actions.after = sbs_common_extend_array(extend->actions.after, after_actions);
        fl_array_free(after_actions);
    }
}

static void merge_compile_target(SbsTargetCompile *extend, const SbsTargetCompileNode *source, const SbsTarget *parent)
{
    extend->includes = sbs_common_extend_array_copy_pointers(extend->includes, source->includes, sbs_common_copy_string);
    extend->sources = sbs_common_extend_array_copy_pointers(extend->sources, source->sources, sbs_common_copy_string);
    extend->defines = sbs_common_extend_array_copy_pointers(extend->defines, source->defines, sbs_common_copy_string);

    if (parent == NULL)
        return;

    if (parent->type == SBS_TARGET_EXECUTABLE)
    {
        SbsTargetExecutable *executable = (SbsTargetExecutable*) parent;
        extend->defines = sbs_common_extend_array_copy_pointers(extend->defines, executable->defines, sbs_common_copy_string);
    }
}

static void merge_archive_target(SbsTargetArchive *extend, const SbsTargetArchiveNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
}

static void merge_shared_target(SbsTargetShared *extend, const SbsTargetSharedNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
}

static void convert_library_node_to_library(void *dest, const void *src, size_t elem_size)
{
    if (!src || !dest)
        return;

    SbsTargetLibraryNode *src_obj = (SbsTargetLibraryNode*)src;

    if (!src_obj)
        return;

    SbsTargetLibrary copy = {
        .name = src_obj->name ? fl_cstring_dup(src_obj->name) : NULL,
        .path = src_obj->path ? fl_cstring_dup(src_obj->path) : NULL
    };

    memcpy(dest, &copy, elem_size);
}

static void merge_executable_target(SbsTargetExecutable *extend, const SbsTargetExecutableNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
    extend->libraries = sbs_common_extend_array_copy_pointers(extend->libraries, source->libraries, convert_library_node_to_library);
    extend->defines = sbs_common_extend_array_copy_pointers(extend->defines, source->defines, sbs_common_copy_string);
}
