#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "toolchain.h"
#include "../utils.h"

static void sbs_toolchain_entry_free(SbsNodeToolchain *toolchain_entry)
{
    if (toolchain_entry->compiler.bin)
        fl_cstring_free(toolchain_entry->compiler.bin);

    if (toolchain_entry->compiler.define_flag)
        fl_cstring_free(toolchain_entry->compiler.define_flag);

    if (toolchain_entry->compiler.include_dir_flag)
        fl_cstring_free(toolchain_entry->compiler.include_dir_flag);

    if (toolchain_entry->archiver.bin)
        fl_cstring_free(toolchain_entry->archiver.bin);

    if (toolchain_entry->linker.bin)
        fl_cstring_free(toolchain_entry->linker.bin);

    if (toolchain_entry->linker.lib_dir_flag)
        fl_cstring_free(toolchain_entry->linker.lib_dir_flag);

    if (toolchain_entry->linker.lib_flag)
        fl_cstring_free(toolchain_entry->linker.lib_flag);

    if (toolchain_entry->for_clause)
        sbs_section_for_free(toolchain_entry->for_clause);
    
    fl_free(toolchain_entry);
}

void sbs_section_toolchain_free(SbsSectionToolchain *toolchain)
{
    fl_cstring_free(toolchain->name);

    if (toolchain->entries)
        fl_array_free_each_pointer(toolchain->entries, (FlArrayFreeElementFunc) sbs_toolchain_entry_free);

    if (toolchain->for_clause)
        sbs_section_for_free(toolchain->for_clause);

    fl_free(toolchain);
}
