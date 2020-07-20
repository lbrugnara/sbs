#include <fllib.h>
#include "configuration.h"
#include "../context.h"
#include "../parser/configuration.h"
#include "../common/common.h"

static void find_ancestors(const SbsSectionConfig *config, FlList *ancestors, const SbsFile *file);
static void merge_compile_config(SbsConfigCompile *extend, const SbsNodeConfigCompile *source);
static void merge_archive_config(SbsConfigArchive *extend, const SbsNodeConfigArchive *source);
static void merge_shared_config(SbsConfigShared *extend, const SbsNodeConfigShared *source);
static void merge_executable_config(SbsConfigExecutable *extend, const SbsNodeConfigExecutable *source);

static void resolve_config_with_hierarchy(SbsContext *context, SbsConfiguration *configuration, const SbsSectionConfig *config_section)
{
    if (config_section->extends)
    {
        for (size_t i = 0; i < fl_array_length(config_section->extends); i++)
        {
            char *parent_name = config_section->extends[i];
            resolve_config_with_hierarchy(context, configuration, fl_hashtable_get(context->file->configurations, parent_name));
        }
    }

    for (size_t i=0; i < fl_array_length(config_section->entries); i++)
    {
        struct SbsNodeConfig *config_node = config_section->entries[i];

        if (config_node->for_clause && !sbs_for_node_eval(config_node->for_clause->condition, context->symbols))
            continue;
        

        merge_compile_config(&configuration->compile, &config_node->compile);
        merge_archive_config(&configuration->archive, &config_node->archive);
        merge_shared_config(&configuration->shared, &config_node->shared);
        merge_executable_config(&configuration->executable, &config_node->executable);
    }
}

SbsConfiguration* sbs_config_resolve(SbsContext *context, const char *config_name)
{
    const SbsSectionConfig *config_section = fl_hashtable_get(context->file->configurations, config_name);

    if (!config_section)
        return NULL;

    SbsConfiguration *configuration = fl_malloc(sizeof(SbsConfiguration));
    configuration->name = fl_cstring_dup(config_section->name);

    resolve_config_with_hierarchy(context, configuration, config_section);
    
    return configuration;
}

void sbs_config_free(SbsConfiguration *config)
{
    fl_cstring_free(config->name);

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

    fl_free(config);
}

static void merge_compile_config(SbsConfigCompile *extend, const SbsNodeConfigCompile *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);
    
    extend->flags = sbs_common_extend_array_copy(extend->flags, source->flags, (SbsArrayCopyElementFn) sbs_common_copy_string);
}

static void merge_archive_config(SbsConfigArchive *extend, const SbsNodeConfigArchive *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy(extend->flags, source->flags, (SbsArrayCopyElementFn) sbs_common_copy_string);
}

static void merge_shared_config(SbsConfigShared *extend, const SbsNodeConfigShared *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy(extend->flags, source->flags, (SbsArrayCopyElementFn) sbs_common_copy_string);
}

static void merge_executable_config(SbsConfigExecutable *extend, const SbsNodeConfigExecutable *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy(extend->flags, source->flags, (SbsArrayCopyElementFn) sbs_common_copy_string);
}

