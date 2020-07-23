#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"
#include "for.h"
#include "../utils.h"

static void sbs_config_entry_free(struct SbsNodeConfig *config)
{
    if (config->compile.flags)
        fl_array_free_each_pointer(config->compile.flags, (FlArrayFreeElementFunc) fl_cstring_free);

    if (config->compile.extension)
        fl_cstring_free(config->compile.extension);


    if (config->archive.flags)
        fl_array_free_each_pointer(config->archive.flags, (FlArrayFreeElementFunc) fl_cstring_free);

    if (config->archive.extension)
        fl_cstring_free(config->archive.extension);


    if (config->shared.flags)
        fl_array_free_each_pointer(config->shared.flags, (FlArrayFreeElementFunc) fl_cstring_free);
    
    if (config->shared.extension)
        fl_cstring_free(config->shared.extension);


    if (config->executable.flags)
        fl_array_free_each_pointer(config->executable.flags, (FlArrayFreeElementFunc) fl_cstring_free);

    if (config->executable.extension)
        fl_cstring_free(config->executable.extension);

    if (config->for_clause)
        sbs_section_for_free(config->for_clause);

    fl_free(config);
}


void sbs_section_config_free(SbsSectionConfig *configuration)
{
    fl_cstring_free(configuration->name);

    if (configuration->extends)
        fl_array_free_each_pointer(configuration->extends, (FlArrayFreeElementFunc) fl_cstring_free);

    if (configuration->entries)
        fl_array_free_each_pointer(configuration->entries, (FlArrayFreeElementFunc) sbs_config_entry_free);

    if (configuration->for_clause)
        sbs_section_for_free(configuration->for_clause);

    fl_free(configuration);
}
