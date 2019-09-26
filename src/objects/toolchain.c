#include "toolchain.h"
#include "../common.h"
#include "../parser/toolchain.h"

struct SbsToolchain* sbs_toolchain_resolve(const struct SbsFile *file, const char *toolchain_name, const char *env_name)
{
    struct SbsToolchainSection *toolchain_section = fl_hashtable_get(file->toolchains, toolchain_name);

    if (!toolchain_section)
        return NULL;

    struct SbsToolchain *toolchain_object = fl_malloc(sizeof(struct SbsToolchain));

    toolchain_object->name = fl_cstring_dup(toolchain_section->name);

    FlList hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(toolchain_section->nodes, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(toolchain_section->nodes, env_name));

    fl_list_prepend(hierarchy, fl_hashtable_get(toolchain_section->nodes, SBS_BASE_OBJECT_KEY));

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsToolchainNode *ancestor = (const struct SbsToolchainNode*)node->value;
        
        toolchain_object->compiler = sbs_common_set_string(toolchain_object->compiler, ancestor->compiler);
        toolchain_object->archiver = sbs_common_set_string(toolchain_object->archiver, ancestor->archiver);
        toolchain_object->linker = sbs_common_set_string(toolchain_object->linker, ancestor->linker);

        node = node->next;
    }

    fl_list_free(hierarchy);

    return toolchain_object;
}

void sbs_toolchain_free(struct SbsToolchain *toolchain)
{
    if (toolchain->name)
        fl_cstring_free(toolchain->name);

    if (toolchain->compiler)
        fl_cstring_free(toolchain->compiler);

    if (toolchain->archiver)
        fl_cstring_free(toolchain->archiver);

    if (toolchain->linker)
        fl_cstring_free(toolchain->linker);
    
    fl_free(toolchain);
}
