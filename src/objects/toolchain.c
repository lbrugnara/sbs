#include "toolchain.h"
#include "../common/common.h"
#include "../parser/toolchain.h"

struct SbsToolchain* sbs_toolchain_resolve(const struct SbsFile *file, const char *toolchain_name, const char *env_name)
{
    struct SbsToolchainSection *toolchain_section = fl_hashtable_get(file->toolchains, toolchain_name);

    if (!toolchain_section)
        return NULL;

    struct SbsToolchain *toolchain_object = fl_malloc(sizeof(struct SbsToolchain));

    toolchain_object->name = fl_cstring_dup(toolchain_section->name);

    FlList *hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(toolchain_section->nodes, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(toolchain_section->nodes, env_name));

    fl_list_prepend(hierarchy, fl_hashtable_get(toolchain_section->nodes, SBS_BASE_OBJECT_KEY));

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsToolchainNode *ancestor = (const struct SbsToolchainNode*)node->value;
        
        toolchain_object->compiler.bin = sbs_common_set_string(toolchain_object->compiler.bin, ancestor->compiler.bin);
        toolchain_object->compiler.include_dir_flag = sbs_common_set_string(toolchain_object->compiler.include_dir_flag, ancestor->compiler.include_dir_flag);
        toolchain_object->compiler.define_flag = sbs_common_set_string(toolchain_object->compiler.define_flag, ancestor->compiler.define_flag);
        
        toolchain_object->archiver.bin = sbs_common_set_string(toolchain_object->archiver.bin, ancestor->archiver.bin);

        toolchain_object->linker.bin = sbs_common_set_string(toolchain_object->linker.bin, ancestor->linker.bin);
        toolchain_object->linker.lib_dir_flag = sbs_common_set_string(toolchain_object->linker.lib_dir_flag, ancestor->linker.lib_dir_flag);
        toolchain_object->linker.lib_flag = sbs_common_set_string(toolchain_object->linker.lib_flag, ancestor->linker.lib_flag);

        node = node->next;
    }

    fl_list_free(hierarchy);

    return toolchain_object;
}

void sbs_toolchain_free(struct SbsToolchain *toolchain)
{
    if (toolchain->name)
        fl_cstring_free(toolchain->name);

    if (toolchain->compiler.bin)
        fl_cstring_free(toolchain->compiler.bin);

    if (toolchain->compiler.include_dir_flag)
        fl_cstring_free(toolchain->compiler.include_dir_flag);

    if (toolchain->compiler.define_flag)
        fl_cstring_free(toolchain->compiler.define_flag);

    if (toolchain->archiver.bin)
        fl_cstring_free(toolchain->archiver.bin);

    if (toolchain->linker.bin)
        fl_cstring_free(toolchain->linker.bin);
    
    if (toolchain->linker.lib_dir_flag)
        fl_cstring_free(toolchain->linker.lib_dir_flag);

    if (toolchain->linker.lib_flag)
        fl_cstring_free(toolchain->linker.lib_flag);
    
    fl_free(toolchain);
}
