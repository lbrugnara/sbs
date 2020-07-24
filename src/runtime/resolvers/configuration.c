#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "configuration.h"
#include "../../utils.h"
#include "../../lang/configuration.h"

static void merge_compile_config(SbsConfigCompile *extend, const SbsNodeConfigCompile *source)
{
    if (source->extension)
        extend->extension = sbs_string_set(extend->extension, source->extension);
    
    extend->flags = sbs_string_array_extend(extend->flags, source->flags);
}

static void merge_archive_config(SbsConfigArchive *extend, const SbsNodeConfigArchive *source)
{
    if (source->extension)
        extend->extension = sbs_string_set(extend->extension, source->extension);

    extend->flags = sbs_string_array_extend(extend->flags, source->flags);
}

static void merge_shared_config(SbsConfigShared *extend, const SbsNodeConfigShared *source)
{
    if (source->extension)
        extend->extension = sbs_string_set(extend->extension, source->extension);

    extend->flags = sbs_string_array_extend(extend->flags, source->flags);
}

static void merge_executable_config(SbsConfigExecutable *extend, const SbsNodeConfigExecutable *source)
{
    if (source->extension)
        extend->extension = sbs_string_set(extend->extension, source->extension);

    extend->flags = sbs_string_array_extend(extend->flags, source->flags);
}

static bool try_resolve_config_with_hierarchy(SbsContext *context, SbsConfiguration *configuration, const SbsSectionConfig *config_section)
{
    if (config_section->for_clause && !sbs_expression_eval(context->symbols, config_section->for_clause->expr))
        return false;

    if (config_section->extends)
    {
        for (size_t i = 0; i < fl_array_length(config_section->extends); i++)
        {
            char *parent_name = config_section->extends[i];
            if (!try_resolve_config_with_hierarchy(context, configuration, fl_hashtable_get(context->file->configurations, parent_name)))
                return false;
        }
    }

    for (size_t i=0; i < fl_array_length(config_section->entries); i++)
    {
        struct SbsNodeConfig *config_node = config_section->entries[i];

        if (config_node->for_clause && !sbs_expression_eval(context->symbols, config_node->for_clause->expr))
            continue;
        

        merge_compile_config(&configuration->compile, &config_node->compile);
        merge_archive_config(&configuration->archive, &config_node->archive);
        merge_shared_config(&configuration->shared, &config_node->shared);
        merge_executable_config(&configuration->executable, &config_node->executable);
    }

    return true;
}

SbsConfiguration* sbs_config_resolve(SbsContext *context, const char *config_name)
{
    const SbsSectionConfig *config_section = fl_hashtable_get(context->file->configurations, config_name);

    if (!config_section)
        return NULL;

    SbsConfiguration *configuration = fl_malloc(sizeof(SbsConfiguration));
    configuration->name = fl_cstring_dup(config_section->name);

    if (!try_resolve_config_with_hierarchy(context, configuration, config_section))
    {
        sbs_config_free(configuration);
        return NULL;
    }
    
    return configuration;
}
