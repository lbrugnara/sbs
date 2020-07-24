#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "toolchain.h"
#include "../../utils.h"
#include "../../lang/toolchain.h"

SbsToolchain* sbs_toolchain_resolve(SbsContext *context, const char *toolchain_name)
{
    SbsSectionToolchain *toolchain_section = fl_hashtable_get(context->file->toolchains, toolchain_name);

    if (!toolchain_section)
        return NULL;

    if (toolchain_section->for_clause && !sbs_expression_eval(context->symbols, toolchain_section->for_clause->expr))
        return NULL;

    SbsToolchain *toolchain_object = fl_malloc(sizeof(SbsToolchain));
    toolchain_object->name = fl_cstring_dup(toolchain_section->name);

    for (size_t i=0; i < fl_array_length(toolchain_section->entries); i++)
    {
        SbsNodeToolchain *toolchain_node = toolchain_section->entries[i];

        if (toolchain_node->for_clause && !sbs_expression_eval(context->symbols, toolchain_node->for_clause->expr))
            continue;
        
        toolchain_object->compiler.bin = sbs_string_set(toolchain_object->compiler.bin, toolchain_node->compiler.bin);
        toolchain_object->compiler.include_dir_flag = sbs_string_set(toolchain_object->compiler.include_dir_flag, toolchain_node->compiler.include_dir_flag);
        toolchain_object->compiler.define_flag = sbs_string_set(toolchain_object->compiler.define_flag, toolchain_node->compiler.define_flag);
        
        toolchain_object->archiver.bin = sbs_string_set(toolchain_object->archiver.bin, toolchain_node->archiver.bin);

        toolchain_object->linker.bin = sbs_string_set(toolchain_object->linker.bin, toolchain_node->linker.bin);
        toolchain_object->linker.lib_dir_flag = sbs_string_set(toolchain_object->linker.lib_dir_flag, toolchain_node->linker.lib_dir_flag);
        toolchain_object->linker.lib_flag = sbs_string_set(toolchain_object->linker.lib_flag, toolchain_node->linker.lib_flag);
    }

    return toolchain_object;
}
