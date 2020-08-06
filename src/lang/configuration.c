#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"
#include "conditional.h"
#include "../utils.h"

static void sbs_config_entry_free(struct SbsNodeConfig *config)
{
    if (config->compile.flags)
        fl_array_free_each_pointer(config->compile.flags, (FlArrayFreeElementFunc) sbs_string_free);

    if (config->compile.extension)
        sbs_string_free(config->compile.extension);


    if (config->archive.flags)
        fl_array_free_each_pointer(config->archive.flags, (FlArrayFreeElementFunc) sbs_string_free);

    if (config->archive.extension)
        sbs_string_free(config->archive.extension);


    if (config->shared.flags)
        fl_array_free_each_pointer(config->shared.flags, (FlArrayFreeElementFunc) sbs_string_free);
    
    if (config->shared.extension)
        sbs_string_free(config->shared.extension);


    if (config->executable.flags)
        fl_array_free_each_pointer(config->executable.flags, (FlArrayFreeElementFunc) sbs_string_free);

    if (config->executable.extension)
        sbs_string_free(config->executable.extension);

    if (config->condition)
        sbs_stmt_conditional_free(config->condition);

    fl_free(config);
}


void sbs_section_config_free(SbsSectionConfig *configuration)
{
    fl_cstring_free(configuration->name);

    if (configuration->extends)
        fl_array_free_each_pointer(configuration->extends, (FlArrayFreeElementFunc) fl_cstring_free);

    if (configuration->entries)
        fl_array_free_each_pointer(configuration->entries, (FlArrayFreeElementFunc) sbs_config_entry_free);

    if (configuration->condition)
        sbs_stmt_conditional_free(configuration->condition);

    fl_free(configuration);
}

SbsSectionConfig* sbs_section_config_new(const struct FlSlice *name)
{
    SbsSectionConfig *configuration = fl_malloc(sizeof(SbsSectionConfig));

    configuration->name = sbs_slice_to_cstring(name);
    configuration->entries = fl_array_new(sizeof(SbsNodeConfig*), 0);

    return configuration;
}

SbsNodeConfig* sbs_section_config_add_node(SbsSectionConfig *config_section)
{
    struct SbsNodeConfig *config_node = fl_malloc(sizeof(struct SbsNodeConfig));

    config_section->entries = fl_array_append(config_section->entries, &config_node);

    return config_node;
}
