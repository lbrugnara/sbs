#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "configuration.h"
#include "string.h"
#include "../../utils.h"
#include "../../lang/configuration.h"
#include "../../lang/string.h"

static void merge_compile_config(SbsContext *context, SbsConfigCompile *extend, const SbsNodeConfigCompile *source)
{
    if (source->extension)
        extend->extension = sbs_cstring_set(extend->extension, source->extension);
    
    extend->flags = sbs_cstring_array_extend_convert(extend->flags, source->flags);
}

static void merge_archive_config(SbsContext *context, SbsConfigArchive *extend, const SbsNodeConfigArchive *source)
{
    if (source->extension)
        extend->extension = sbs_cstring_set(extend->extension, source->extension);

    extend->flags = sbs_cstring_array_extend(extend->flags, source->flags);
}

static void merge_shared_config(SbsContext *context, SbsConfigShared *extend, const SbsNodeConfigShared *source)
{
    if (source->extension)
        extend->extension = sbs_cstring_set(extend->extension, source->extension);

    extend->flags = sbs_cstring_array_extend(extend->flags, source->flags);
}

static void merge_executable_config(SbsContext *context, SbsConfigExecutable *extend, const SbsNodeConfigExecutable *source)
{
    if (source->extension)
        extend->extension = sbs_cstring_set(extend->extension, source->extension);

    extend->flags = sbs_cstring_array_extend(extend->flags, source->flags);
}

static bool try_resolve_config_with_hierarchy(SbsContext *context, SbsConfiguration *configuration, const SbsSectionConfig *config_section)
{
    if (config_section->condition && !sbs_expression_eval_bool(context->symbols, config_section->condition->expr))
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

        if (config_node->condition && !sbs_expression_eval_bool(context->symbols, config_node->condition->expr))
            continue;
        

        merge_compile_config(context, &configuration->compile, &config_node->compile);
        merge_archive_config(context, &configuration->archive, &config_node->archive);
        merge_shared_config(context, &configuration->shared, &config_node->shared);
        merge_executable_config(context, &configuration->executable, &config_node->executable);
    }

    return true;
}

SbsConfiguration* sbs_config_resolve(SbsContext *context, const char *config_name)
{
    const SbsSectionConfig *config_section = fl_hashtable_get(context->file->configurations, config_name);

    if (!config_section)
        return NULL;

    SbsConfiguration *configuration = sbs_config_new(config_section->name);

    if (!try_resolve_config_with_hierarchy(context, configuration, config_section))
    {
        sbs_config_free(configuration);
        return NULL;
    }
    
    return configuration;
}
