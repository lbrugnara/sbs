#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-shared.h"
#include "../utils.h"

SbsSectionShared* sbs_section_shared_new(const struct FlSlice *name)
{
    SbsSectionShared *section = fl_malloc(sizeof(SbsSectionShared));

    section->base.type = SBS_SECTION_TARGET_SHARED;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeShared*), 0);

    return section;
}

SbsNodeShared* sbs_section_shared_add_node(SbsSectionShared *section)
{
    SbsNodeShared *node = fl_malloc(sizeof(SbsNodeShared));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

void sbs_node_shared_free(SbsNodeShared *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);
    
    if (node->objects)
        fl_array_free_each_pointer(node->objects, (FlArrayFreeElementFunc) sbs_value_source_free);

    if (node->output_name)
        fl_cstring_free(node->output_name);

    fl_free(node);
}

void sbs_section_shared_free(SbsSectionShared *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_shared_free);

    fl_free(section);
}
