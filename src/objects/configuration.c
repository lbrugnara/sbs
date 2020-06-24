#include <fllib.h>
#include "configuration.h"
#include "../parser/configuration.h"
#include "../common/common.h"

static void find_ancestors(const struct SbsConfigSection *config, FlList *ancestors, const struct SbsFile *file, const char *env_name);
static void merge_compile_config(struct SbsConfigCompile *extend, const struct SbsConfigCompileNode *source);
static void merge_archive_config(struct SbsConfigArchive *extend, const struct SbsConfigArchiveNode *source);
static void merge_shared_config(struct SbsConfigShared *extend, const struct SbsConfigSharedNode *source);
static void merge_executable_config(struct SbsConfigExecutable *extend, const struct SbsConfigExecutableNode *source);

struct SbsConfiguration* sbs_config_resolve(const struct SbsFile *file, const char *config_name, const char *env_name)
{    
    const struct SbsConfigSection *config_section = fl_hashtable_get(file->configurations, config_name);

    if (!config_section)
        return NULL;

    struct SbsConfiguration *config_object = fl_malloc(sizeof(struct SbsConfiguration));
    config_object->name = fl_cstring_dup(config_section->name);

    FlList *hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(config_section->nodes, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(config_section->nodes, env_name));
    fl_list_prepend(hierarchy, fl_hashtable_get(config_section->nodes, SBS_BASE_OBJECT_KEY));
    find_ancestors(config_section, hierarchy, file, env_name);

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsConfigNode *ancestor = (const struct SbsConfigNode*)node->value;

        // struct SbsConfigCompileNode compile;
        merge_compile_config(&config_object->compile, &ancestor->compile);
        merge_archive_config(&config_object->archive, &ancestor->archive);
        merge_shared_config(&config_object->shared, &ancestor->shared);
        merge_executable_config(&config_object->executable, &ancestor->executable);

        node = node->next;
    }

    fl_list_free(hierarchy);

    return config_object;
}

void sbs_config_free(struct SbsConfiguration *config)
{
    fl_cstring_free(config->name);

    if (config->compile.flags)
        fl_array_free_each(config->compile.flags, sbs_common_free_string);

    if (config->compile.extension)
        fl_cstring_free(config->compile.extension);


    if (config->archive.flags)
        fl_array_free_each(config->archive.flags, sbs_common_free_string);

    if (config->archive.extension)
        fl_cstring_free(config->archive.extension);


    if (config->shared.flags)
        fl_array_free_each(config->shared.flags, sbs_common_free_string);
    
    if (config->shared.extension)
        fl_cstring_free(config->shared.extension);


    if (config->executable.flags)
        fl_array_free_each(config->executable.flags, sbs_common_free_string);

    if (config->executable.extension)
        fl_cstring_free(config->executable.extension);


    fl_free(config);
}


static void find_ancestors(const struct SbsConfigSection *config, FlList *ancestors, const struct SbsFile *file, const char *env_name)
{
    if (!config->extends)
        return;

    size_t length = fl_array_length(config->extends);
    for (size_t i=0; i < length; i++)
    {
        struct SbsConfigSection *parent_config = fl_hashtable_get(file->configurations, config->extends[i]);

        if (fl_hashtable_has_key(parent_config->nodes, env_name))
            fl_list_prepend(ancestors, fl_hashtable_get(parent_config->nodes, env_name));    

        fl_list_prepend(ancestors, fl_hashtable_get(parent_config->nodes, SBS_BASE_OBJECT_KEY));

    }
    
    for (size_t i=0; i < length; i++)
        find_ancestors(fl_hashtable_get(file->configurations, config->extends[i]), ancestors, file, env_name);
}

static void merge_compile_config(struct SbsConfigCompile *extend, const struct SbsConfigCompileNode *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);
    
    extend->flags = sbs_common_extend_array_copy_pointers(extend->flags, source->flags, sbs_common_copy_string);
}

static void merge_archive_config(struct SbsConfigArchive *extend, const struct SbsConfigArchiveNode *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy_pointers(extend->flags, source->flags, sbs_common_copy_string);
}

static void merge_shared_config(struct SbsConfigShared *extend, const struct SbsConfigSharedNode *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy_pointers(extend->flags, source->flags, sbs_common_copy_string);
}

static void merge_executable_config(struct SbsConfigExecutable *extend, const struct SbsConfigExecutableNode *source)
{
    if (source->extension)
        extend->extension = sbs_common_set_string(extend->extension, source->extension);

    extend->flags = sbs_common_extend_array_copy_pointers(extend->flags, source->flags, sbs_common_copy_string);
}

