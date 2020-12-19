#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "toolchain.h"
#include "../utils.h"
#include "../lang/toolchain.h"

SbsToolchain* sbs_toolchain_new(const char *name)
{
    SbsToolchain *toolchain_object = fl_malloc(sizeof(SbsToolchain));

    toolchain_object->name = fl_cstring_dup(name);

    return toolchain_object;
}

void sbs_toolchain_free(SbsToolchain *toolchain)
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

SbsToolchain* sbs_toolchain_resolve(SbsResolveContext *context, const char *toolchain_name)
{
    SbsSectionToolchain *toolchain_section = fl_hashtable_get(context->file->toolchains, toolchain_name);

    if (!toolchain_section)
        return NULL;

    if (toolchain_section->condition && !sbs_eval_expr_to_bool(context->evalctx, toolchain_section->condition))
        return NULL;

    SbsToolchain *toolchain_object = sbs_toolchain_new(toolchain_section->name);

    for (size_t i=0; i < fl_array_length(toolchain_section->entries); i++)
    {
        SbsNodeToolchain *toolchain_node = toolchain_section->entries[i];

        if (toolchain_node->condition && !sbs_eval_expr_to_bool(context->evalctx, toolchain_node->condition))
            continue;
        
        toolchain_object->compiler.bin = sbs_cstring_set(toolchain_object->compiler.bin, toolchain_node->compiler.bin);
        toolchain_object->compiler.include_dir_flag = sbs_cstring_set(toolchain_object->compiler.include_dir_flag, toolchain_node->compiler.include_dir_flag);
        toolchain_object->compiler.define_flag = sbs_cstring_set(toolchain_object->compiler.define_flag, toolchain_node->compiler.define_flag);
        
        toolchain_object->archiver.bin = sbs_cstring_set(toolchain_object->archiver.bin, toolchain_node->archiver.bin);

        toolchain_object->linker.bin = sbs_cstring_set(toolchain_object->linker.bin, toolchain_node->linker.bin);
        toolchain_object->linker.lib_dir_flag = sbs_cstring_set(toolchain_object->linker.lib_dir_flag, toolchain_node->linker.lib_dir_flag);
        toolchain_object->linker.lib_flag = sbs_cstring_set(toolchain_object->linker.lib_flag, toolchain_node->linker.lib_flag);
    }

    sbs_eval_context_add_variable(context->evalctx, "sbs.toolchain", toolchain_object->name);

    if (toolchain_object->compiler.bin != NULL)
        sbs_eval_context_add_variable(context->evalctx, "sbs.compiler", toolchain_object->compiler.bin);

    if (toolchain_object->archiver.bin != NULL)
        sbs_eval_context_add_variable(context->evalctx, "sbs.archiver", toolchain_object->archiver.bin);

    if (toolchain_object->linker.bin != NULL)
        sbs_eval_context_add_variable(context->evalctx, "sbs.linker", toolchain_object->linker.bin);

    return toolchain_object;
}
