#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-executable.h"
#include "../utils.h"

SbsSectionExecutable* sbs_section_executable_new(const struct FlSlice *name)
{
    SbsSectionExecutable *section = fl_malloc(sizeof(SbsSectionExecutable));

    section->base.type = SBS_TARGET_EXECUTABLE;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeExecutable*), 0);

    return section;
}

SbsNodeExecutable* sbs_section_executable_add_node(SbsSectionExecutable *section)
{
    SbsNodeExecutable *node = fl_malloc(sizeof(SbsNodeExecutable));

    section->entries = fl_array_append(section->entries, &node);

    return node;
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

void sbs_node_executable_free(SbsNodeExecutable *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);

    if (node->objects)
        fl_array_free_each(node->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (node->libraries)
        fl_array_free_each(node->libraries, free_library_node);

    if (node->output_name)
        fl_cstring_free(node->output_name);

    if (node->defines)
        fl_array_free_each_pointer(node->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(node);
}

void sbs_section_executable_free(SbsSectionExecutable *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_executable_free);

    fl_free(section);
}
