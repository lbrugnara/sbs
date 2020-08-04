#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-compile.h"
#include "../utils.h"

SbsSectionCompile* sbs_section_compile_new(const struct FlSlice *name)
{
    SbsSectionCompile *section = fl_malloc(sizeof(SbsSectionCompile));

    section->base.type = SBS_SECTION_TARGET_COMPILE;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeCompile*), 0);

    return section;
}

SbsNodeCompile* sbs_section_compile_add_node(SbsSectionCompile *section)
{
    SbsNodeCompile *node = fl_malloc(sizeof(SbsNodeCompile));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

void sbs_node_compile_free(SbsNodeCompile *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);
            
    if (node->sources)
        fl_array_free_each_pointer(node->sources, (FlArrayFreeElementFunc) fl_cstring_free);

    if (node->includes)
        fl_array_free_each_pointer(node->includes, (FlArrayFreeElementFunc) fl_cstring_free);

    if (node->defines)
        fl_array_free_each_pointer(node->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(node);
}

void sbs_section_compile_free(SbsSectionCompile *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_compile_free);

    fl_free(section);
}
