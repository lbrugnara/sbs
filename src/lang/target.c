#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "../utils.h"

SbsSectionCompile* sbs_section_compile_new(const struct FlSlice *name)
{
    SbsSectionCompile *target_section = fl_malloc(sizeof(SbsSectionCompile));

    target_section->base.type = SBS_TARGET_COMPILE;
    target_section->base.name = sbs_slice_to_str(name);
    target_section->entries = fl_array_new(sizeof(SbsNodeCompile*), 0);

    return target_section;
}

SbsSectionArchive* sbs_section_archive_new(const struct FlSlice *name)
{
    SbsSectionArchive *target_section = fl_malloc(sizeof(SbsSectionArchive));

    target_section->base.type = SBS_TARGET_ARCHIVE;
    target_section->base.name = sbs_slice_to_str(name);
    target_section->entries = fl_array_new(sizeof(SbsNodeArchive*), 0);

    return target_section;
}

SbsSectionShared* sbs_section_shared_new(const struct FlSlice *name)
{
    SbsSectionShared *target_section = fl_malloc(sizeof(SbsSectionShared));

    target_section->base.type = SBS_TARGET_SHARED;
    target_section->base.name = sbs_slice_to_str(name);
    target_section->entries = fl_array_new(sizeof(SbsNodeShared*), 0);

    return target_section;
}

SbsSectionExecutable* sbs_section_executable_new(const struct FlSlice *name)
{
    SbsSectionExecutable *target_section = fl_malloc(sizeof(SbsSectionExecutable));

    target_section->base.type = SBS_TARGET_EXECUTABLE;
    target_section->base.name = sbs_slice_to_str(name);
    target_section->entries = fl_array_new(sizeof(SbsNodeExecutable*), 0);

    return target_section;
}

SbsNodeCompile* sbs_section_compile_add_node(SbsSectionCompile *target_section)
{
    SbsNodeCompile *target_entry = fl_malloc(sizeof(SbsNodeCompile));

    target_section->entries = fl_array_append(target_section->entries, &target_entry);

    return target_entry;
}

SbsNodeArchive* sbs_section_archive_add_node(SbsSectionArchive *target_section)
{
    SbsNodeArchive *target_entry = fl_malloc(sizeof(SbsNodeArchive));

    target_section->entries = fl_array_append(target_section->entries, &target_entry);

    return target_entry;
}

SbsNodeShared* sbs_section_shared_add_node(SbsSectionShared *target_section)
{
    SbsNodeShared *target_entry = fl_malloc(sizeof(SbsNodeShared));

    target_section->entries = fl_array_append(target_section->entries, &target_entry);

    return target_entry;
}

SbsNodeExecutable* sbs_section_executable_add_node(SbsSectionExecutable *target_section)
{
    SbsNodeExecutable *target_entry = fl_malloc(sizeof(SbsNodeExecutable));

    target_section->entries = fl_array_append(target_section->entries, &target_entry);

    return target_entry;
}

static void sbs_node_compile_free(SbsNodeCompile *target_entry)
{
    sbs_property_actions_free(&target_entry->base.actions);

    if (target_entry->base.condition)
        sbs_stmt_conditional_free(target_entry->base.condition);

    if (target_entry->base.output_dir)
        fl_cstring_free(target_entry->base.output_dir);
            
    if (target_entry->sources)
        fl_array_free_each_pointer(target_entry->sources, (FlArrayFreeElementFunc) fl_cstring_free);

    if (target_entry->includes)
        fl_array_free_each_pointer(target_entry->includes, (FlArrayFreeElementFunc) fl_cstring_free);

    if (target_entry->defines)
        fl_array_free_each_pointer(target_entry->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(target_entry);
}

static void sbs_section_compile_free(SbsSectionCompile *target_section)
{
    if (target_section->entries)
        fl_array_free_each_pointer(target_section->entries, (FlArrayFreeElementFunc) sbs_node_compile_free);
}

static void sbs_node_archive_free(SbsSectionTargetType target_type, SbsNodeArchive *target_entry)
{
    sbs_property_actions_free(&target_entry->base.actions);

    if (target_entry->base.condition)
        sbs_stmt_conditional_free(target_entry->base.condition);

    if (target_entry->base.output_dir)
        fl_cstring_free(target_entry->base.output_dir);

    if (target_entry->objects)
        fl_array_free_each(target_entry->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target_entry->output_name)
        fl_cstring_free(target_entry->output_name);    

    fl_free(target_entry);
}

static void sbs_section_archive_free(SbsSectionArchive *target_section)
{
    if (target_section->entries)
        fl_array_free_each_pointer(target_section->entries, (FlArrayFreeElementFunc) sbs_node_archive_free);
}

static void sbs_node_shared_free(SbsNodeShared *target_entry)
{
    sbs_property_actions_free(&target_entry->base.actions);

    if (target_entry->base.condition)
        sbs_stmt_conditional_free(target_entry->base.condition);

    if (target_entry->base.output_dir)
        fl_cstring_free(target_entry->base.output_dir);
    
    if (target_entry->objects)
        fl_array_free_each(target_entry->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target_entry->output_name)
        fl_cstring_free(target_entry->output_name);

    fl_free(target_entry);
}

static void sbs_section_shared_free(SbsSectionShared *target_section)
{
    if (target_section->entries)
        fl_array_free_each_pointer(target_section->entries, (FlArrayFreeElementFunc) sbs_node_shared_free);
}

static void free_library_node(void *obj)
{
    if (!obj)
        return;

    SbsPropertyLibrary *lib = (SbsPropertyLibrary*)obj;

    if (lib->name)
        fl_cstring_free(lib->name);

    if (lib->path)
        fl_cstring_free(lib->path);
}

static void sbs_node_executable_free(SbsNodeExecutable *target_entry)
{
    sbs_property_actions_free(&target_entry->base.actions);

    
    if (target_entry->base.condition)
        sbs_stmt_conditional_free(target_entry->base.condition);

    if (target_entry->base.output_dir)
        fl_cstring_free(target_entry->base.output_dir);

    if (target_entry->objects)
        fl_array_free_each(target_entry->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (target_entry->libraries)
        fl_array_free_each(target_entry->libraries, free_library_node);

    if (target_entry->output_name)
        fl_cstring_free(target_entry->output_name);

    if (target_entry->defines)
        fl_array_free_each_pointer(target_entry->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(target_entry);
}

static void sbs_section_executable_free(SbsSectionExecutable *target_section)
{
    if (target_section->entries)
        fl_array_free_each_pointer(target_section->entries, (FlArrayFreeElementFunc) sbs_node_executable_free);
}

void sbs_section_target_free(SbsAbstractSectionTarget *target_section)
{
    if (target_section->name)
        fl_cstring_free(target_section->name);

    switch (target_section->type)
    {
        case SBS_TARGET_COMPILE:
            sbs_section_compile_free((SbsSectionCompile*) target_section);
            break;
        case SBS_TARGET_ARCHIVE:
            sbs_section_archive_free((SbsSectionArchive*) target_section);
            break;
        case SBS_TARGET_SHARED:
            sbs_section_shared_free((SbsSectionShared*) target_section);
            break;
        case SBS_TARGET_EXECUTABLE:
            sbs_section_executable_free((SbsSectionExecutable*) target_section);
            break;
    }

    fl_free(target_section);
}
