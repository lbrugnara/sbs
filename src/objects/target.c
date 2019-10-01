#include <fllib.h>
#include "target.h"
#include "../common.h"
#include "../parser/target.h"
#include "../parser/action.h"

static void merge_base_target(const struct SbsFile *file, const char *env_name, struct SbsTarget *extend, const struct SbsTargetNode *source);
static void merge_compile_target(struct SbsTargetCompile *extend, const struct SbsTargetCompileNode *source);
static void merge_archive_target(struct SbsTargetArchive *extend, const struct SbsTargetArchiveNode *source);
static void merge_shared_target(struct SbsTargetShared *extend, const struct SbsTargetSharedNode *source);
static void merge_executable_target(struct SbsTargetExecutable *extend, const struct SbsTargetExecutableNode *source);

struct SbsTarget* sbs_target_resolve(const struct SbsFile *file, const char *target_name, const char *env_name)
{    
    const struct SbsTargetSection *target_section = fl_hashtable_get(file->targets, target_name);

    if (!target_section)
        return NULL;

    struct SbsTarget *target_obj = NULL;
    switch (target_section->type)
    {
        case SBS_TARGET_COMPILE:
            target_obj = fl_malloc(sizeof(struct SbsTargetCompile));
            break;
        case SBS_TARGET_ARCHIVE:
            target_obj = fl_malloc(sizeof(struct SbsTargetArchive));
            break;
        case SBS_TARGET_SHARED:
            target_obj = fl_malloc(sizeof(struct SbsTargetShared));
            break;
        case SBS_TARGET_EXECUTABLE:
            target_obj = fl_malloc(sizeof(struct SbsTargetExecutable));
            break;
        default:
            return NULL;
    }

    target_obj->name = fl_cstring_dup(target_section->name);
    target_obj->type = target_section->type;

    FlList hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(target_section->entries, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, env_name));

    fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, SBS_BASE_OBJECT_KEY));

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsTargetNode *ancestor = (const struct SbsTargetNode*)node->value;
        
        merge_base_target(file, env_name, target_obj, ancestor);

        switch (target_section->type)
        {
            case SBS_TARGET_COMPILE:
                merge_compile_target((struct SbsTargetCompile*)target_obj, (struct SbsTargetCompileNode*)node->value);
                break;
            case SBS_TARGET_ARCHIVE:
                merge_archive_target((struct SbsTargetArchive*)target_obj, (struct SbsTargetArchiveNode*)node->value);
                break;
            case SBS_TARGET_SHARED:
                merge_shared_target((struct SbsTargetShared*)target_obj, (struct SbsTargetSharedNode*)node->value);
                break;
            case SBS_TARGET_EXECUTABLE:
                merge_executable_target((struct SbsTargetExecutable*)target_obj, (struct SbsTargetExecutableNode*)node->value);
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

    struct SbsTargetLibrary *lib = (struct SbsTargetLibrary*)obj;

    if (lib->name)
        fl_cstring_free(lib->name);

    if (lib->path)
        fl_cstring_free(lib->path);
}

void sbs_target_free(struct SbsTarget *target)
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
            struct SbsTargetCompile *compile = (struct SbsTargetCompile*)target;
            
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
            struct SbsTargetArchive *archive = (struct SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_free_each(archive->objects, sbs_common_free_string_or_id);

            if (archive->output_name)
                fl_cstring_free(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            struct SbsTargetShared *shared = (struct SbsTargetShared*)target;

            if (shared->objects)
                fl_array_free_each(shared->objects, sbs_common_free_string_or_id);

            if (shared->output_name)
                fl_cstring_free(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            struct SbsTargetExecutable *executable = (struct SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_free_each(executable->objects, sbs_common_free_string_or_id);

            if (executable->libraries)
                fl_array_free_each(executable->libraries, free_library);

            if (executable->output_name)
                fl_cstring_free(executable->output_name);

            break;
        }
    }

    fl_free(target);
}

static void merge_base_target(const struct SbsFile *file, const char *env_name, struct SbsTarget *extend, const struct SbsTargetNode *source)
{
    extend->output_dir = sbs_common_set_string(extend->output_dir, source->output_dir);

    struct SbsAction **before_actions = sbs_action_resolve_all(file, source->actions.before, env_name);
    if (before_actions)
    {
        extend->actions.before = sbs_common_extend_array(extend->actions.before, before_actions);
        fl_array_free(before_actions);
    }

    struct SbsAction **after_actions = sbs_action_resolve_all(file, source->actions.after, env_name);
    if (after_actions)
    {
        extend->actions.after = sbs_common_extend_array(extend->actions.after, after_actions);
        fl_array_free(after_actions);
    }
}

static void merge_compile_target(struct SbsTargetCompile *extend, const struct SbsTargetCompileNode *source)
{
    extend->includes = sbs_common_extend_array_copy_pointers(extend->includes, source->includes, sbs_common_copy_string);
    extend->sources = sbs_common_extend_array_copy_pointers(extend->sources, source->sources, sbs_common_copy_string);
    extend->defines = sbs_common_extend_array_copy_pointers(extend->defines, source->defines, sbs_common_copy_string);
}

static void merge_archive_target(struct SbsTargetArchive *extend, const struct SbsTargetArchiveNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
}

static void merge_shared_target(struct SbsTargetShared *extend, const struct SbsTargetSharedNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
}

static void convert_library_node_to_library(void *dest, const void *src, size_t elem_size)
{
    if (!src || !dest)
        return;

    struct SbsTargetLibraryNode *src_obj = (struct SbsTargetLibraryNode*)src;

    if (!src_obj)
        return;

    struct SbsTargetLibrary copy = {
        .name = fl_cstring_dup(src_obj->name),
        .path = fl_cstring_dup(src_obj->path)
    };

    memcpy(dest, &copy, elem_size);
}

static void merge_executable_target(struct SbsTargetExecutable *extend, const struct SbsTargetExecutableNode *source)
{
    extend->output_name = sbs_common_set_string(extend->output_name, source->output_name);
    extend->objects = sbs_common_extend_array_copy_pointers(extend->objects, source->objects, sbs_common_copy_string_or_id);
    extend->libraries = sbs_common_extend_array_copy_pointers(extend->libraries, source->libraries, convert_library_node_to_library);
}
