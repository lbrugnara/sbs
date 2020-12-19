#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"
#include "../utils.h"
#include "../lang/expr-string.h"
#include "../lang/configuration.h"

SbsConfiguration* sbs_config_new(const char *name)
{
    SbsConfiguration *configuration = fl_malloc(sizeof(SbsConfiguration));

    configuration->name = fl_cstring_dup(name);

    return configuration;
}

void sbs_config_free(SbsConfiguration *config)
{
    fl_cstring_free(config->name);

    if (config->compile.flags)
        fl_array_free_each_pointer(config->compile.flags, (FlArrayFreeElementFunc) sbs_expr_free_string);

    if (config->compile.extension)
        sbs_expr_free_string(config->compile.extension);


    if (config->archive.flags)
        fl_array_free_each_pointer(config->archive.flags, (FlArrayFreeElementFunc) sbs_expr_free_string);

    if (config->archive.extension)
        sbs_expr_free_string(config->archive.extension);


    if (config->shared.flags)
        fl_array_free_each_pointer(config->shared.flags, (FlArrayFreeElementFunc) sbs_expr_free_string);
    
    if (config->shared.extension)
        sbs_expr_free_string(config->shared.extension);


    if (config->executable.flags)
        fl_array_free_each_pointer(config->executable.flags, (FlArrayFreeElementFunc) sbs_expr_free_string);

    if (config->executable.extension)
        sbs_expr_free_string(config->executable.extension);

    fl_free(config);
}

static void merge_compile_config(SbsResolveContext *context, SbsConfigCompile *extend, const SbsNodeConfigCompile *source)
{
    extend->extension = sbs_expr_set_string(extend->extension, source->extension);
    extend->flags = sbs_expr_extend_string_array(extend->flags, source->flags);
}

static void merge_archive_config(SbsResolveContext *context, SbsConfigArchive *extend, const SbsNodeConfigArchive *source)
{
    extend->extension = sbs_expr_set_string(extend->extension, source->extension);
    extend->flags = sbs_expr_extend_string_array(extend->flags, source->flags);
}

static void merge_shared_config(SbsResolveContext *context, SbsConfigShared *extend, const SbsNodeConfigShared *source)
{
    extend->extension = sbs_expr_set_string(extend->extension, source->extension);
    extend->flags = sbs_expr_extend_string_array(extend->flags, source->flags);
}

static void merge_executable_config(SbsResolveContext *context, SbsConfigExecutable *extend, const SbsNodeConfigExecutable *source)
{
    extend->extension = sbs_expr_set_string(extend->extension, source->extension);
    extend->flags = sbs_expr_extend_string_array(extend->flags, source->flags);
}

static bool try_resolve_config_with_hierarchy(SbsResolveContext *context, SbsConfiguration *configuration, const SbsSectionConfig *config_section)
{
    if (config_section->condition && !sbs_eval_expr_to_bool(context->evalctx, config_section->condition))
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

        if (config_node->condition && !sbs_eval_expr_to_bool(context->evalctx, config_node->condition))
            continue;
        

        merge_compile_config(context, &configuration->compile, &config_node->compile);
        merge_archive_config(context, &configuration->archive, &config_node->archive);
        merge_shared_config(context, &configuration->shared, &config_node->shared);
        merge_executable_config(context, &configuration->executable, &config_node->executable);
    }

    return true;
}

SbsConfiguration* sbs_config_resolve(SbsResolveContext *context, const char *config_name)
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

    sbs_eval_context_add_variable(context->evalctx, "sbs.config", configuration->name);
    
    return configuration;
}
