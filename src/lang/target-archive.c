#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-archive.h"
#include "../utils.h"

SbsSectionArchive* sbs_section_archive_new(const struct FlSlice *name)
{
    SbsSectionArchive *section = fl_malloc(sizeof(SbsSectionArchive));

    section->base.type = SBS_TARGET_ARCHIVE;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeArchive*), 0);

    return section;
}

SbsNodeArchive* sbs_section_archive_add_node(SbsSectionArchive *section)
{
    SbsNodeArchive *node = fl_malloc(sizeof(SbsNodeArchive));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

void sbs_node_archive_free(SbsNodeArchive *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);

    if (node->objects)
        fl_array_free_each(node->objects, (FlArrayFreeElementFunc) sbs_value_command_free);

    if (node->output_name)
        fl_cstring_free(node->output_name);    

    fl_free(node);
}

void sbs_section_archive_free(SbsSectionArchive *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_archive_free);

    fl_free(section);
}
