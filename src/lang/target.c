#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "../utils.h"

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

static void sbs_target_entry_free(SbsSectionTargetType target_type, SbsSectionTarget *target_entry)
{
    sbs_property_actions_free(&target_entry->actions);

    if (target_entry->output_dir)
        fl_cstring_free(target_entry->output_dir);

    switch (target_type)
    {
        case SBS_TARGET_COMPILE:
        {
            SbsSectionCompile *compile = (SbsSectionCompile*)target_entry;
            
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
            SbsSectionArchive *archive = (SbsSectionArchive*)target_entry;

            if (archive->objects)
                fl_array_free_each(archive->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (archive->output_name)
                fl_cstring_free(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            SbsSectionShared *shared = (SbsSectionShared*)target_entry;

            if (shared->objects)
                fl_array_free_each(shared->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (shared->output_name)
                fl_cstring_free(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            SbsSectionExecutable *executable = (SbsSectionExecutable*)target_entry;

            if (executable->objects)
                fl_array_free_each(executable->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

            if (executable->libraries)
                fl_array_free_each(executable->libraries, free_library_node);

            if (executable->output_name)
                fl_cstring_free(executable->output_name);

            if (executable->defines)
                fl_array_free_each_pointer(executable->defines, (FlArrayFreeElementFunc) fl_cstring_free);

            break;
        }
    }

    if (target_entry->for_clause)
        sbs_section_for_free(target_entry->for_clause);

    fl_free(target_entry);
}

void sbs_section_target_free(SbsAbstractSectionTarget *target_section)
{
    fl_cstring_free(target_section->name);

    if (target_section->entries)
    {
        for (size_t i = 0; i < fl_array_length(target_section->entries); i++)
        {
           SbsSectionTarget *target_entry = target_section->entries[i];
           sbs_target_entry_free(target_section->type, target_entry);
        }
        fl_array_free(target_section->entries);
    }

    fl_free(target_section);
}
