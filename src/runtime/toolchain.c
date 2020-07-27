#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "toolchain.h"

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
