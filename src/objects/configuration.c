#include <fllib.h>
#include "configuration.h"
#include "common.h"
#include "parser.h"

#define BASE_CONFIG_KEY "#base"

void sbs_config_entry_free(struct SbsConfigEntry *config)
{
    if (config->compile.flags)
        fl_array_free_each(config->compile.flags, sbs_common_free_string);

    if (config->compile.include_dir_flag)
        fl_cstring_free(config->compile.include_dir_flag);

    if (config->compile.define_flag)
        fl_cstring_free(config->compile.define_flag);

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


void sbs_config_free(struct SbsConfigSection *configuration)
{
    fl_cstring_free(configuration->name);

    if (configuration->extends)
        fl_array_free_each(configuration->extends, sbs_common_free_string);

    if (configuration->entries)
    {
        // We share the same struct SbsConfigEntry between multiple environments, so we nullify 
        // duplicates, release the objects, and finally release the memory used by the array returned
        // by fl_hashtable_values
        struct SbsConfigEntry **configurations = fl_hashtable_values(configuration->entries);

        size_t count = fl_array_length(configurations);
        for (size_t i=0; i < count; i++)
        {
            for (size_t j=0; j < count; j++)
            {
                if (i == j)
                    continue;

                if (configurations[i] == configurations[j])
                    configurations[j] = NULL;
            }
        }

        for (size_t i=0; i < count; i++)
        {
            if (configurations[i] == NULL)
                continue;

            sbs_config_entry_free(configurations[i]);
        }

        fl_array_free(configurations);

        // Delete the hashtable including the keys (we already deleted the values)
        fl_hashtable_free(configuration->entries);
    }

    fl_free(configuration);
}

static void parse_compile_block(struct SbsParser *parser, struct SbsConfigCompile *compile)
{
    const struct SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->flags = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"include_dir_flag", 16))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->include_dir_flag = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"define_flag", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->define_flag = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->extension = sbs_common_parse_string(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing a compile configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_archive_block(struct SbsParser *parser, struct SbsConfigArchive *archive)
{
    const struct SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            archive->flags = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            archive->extension = sbs_common_parse_string(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing an archive configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_shared_block(struct SbsParser *parser, struct SbsConfigShared *shared)
{
    const struct SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            shared->flags = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            shared->extension = sbs_common_parse_string(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing a shared configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_executable_block(struct SbsParser *parser, struct SbsConfigExecutable *executable)
{
    const struct SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            executable->flags = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            executable->extension = sbs_common_parse_string(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing an executable configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_config_body(struct SbsParser *parser, struct SbsConfigEntry *configuration)
{
    const struct SbsToken *token = NULL;

    while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_FOR)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"compile", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_compile_block(parser, &configuration->compile);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"archive", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_archive_block(parser, &configuration->archive);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"shared", 6))
        {
            sbs_parser_consume(parser, SBS_TOKEN_SHARED);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_shared_block(parser, &configuration->shared);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"executable", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_executable_block(parser, &configuration->executable);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else
        {
            sbs_parser_error(token, "while parsing a configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_configuration_parse
 *  Parses a *config* block which supports the following properties:
 *      - compile: A section for the compile target configuration which supports:
 *          - flags: Compile flags to pass to the executable
 *          - include_dir_flag: Flag to pass an include dir to the compiler
 *          - extension: Target's output extension
 *          - defines: Defines to pass to the compiler
 *      - archive: A section for the archive target configuration which supports:
 *          - flags: Archive flags to pass to the executable
 *          - extension: Target's output extension
 *      - shared: A section for the shared target configuration which supports:
 *          - flags: Shared flags to pass to the executable
 *          - extension: Target's output extension
 *      - executable: A section for the executable target configuration which supports:
 *          - flags: Executable flags to pass to the executable
 *          - extension: Target's output extension
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsConfigSection* - The parsed *config* block
 *
 */
struct SbsConfigSection* sbs_config_parse(struct SbsParser *parser)
{
    struct SbsConfigSection *configuration = fl_malloc(sizeof(struct SbsConfigSection));

    configuration->entries = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsConfigEntry *base_config_entry = fl_malloc(sizeof(struct SbsConfigEntry));

    // Use the special key #base for the default config
    fl_hashtable_add(configuration->entries, BASE_CONFIG_KEY, base_config_entry);

    // Consume 'configuration'
    sbs_parser_consume(parser, SBS_TOKEN_CONFIG);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    configuration->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
        configuration->extends = sbs_common_parse_extends_declaration(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = sbs_parser_peek(parser);

        if (token->type == SBS_TOKEN_FOR)
        {
            // Parse the for declaration
            char **envs = sbs_common_parse_for_declaration(parser);

            // Parse the configuration info
            struct SbsConfigEntry *configuration_info = fl_malloc(sizeof(struct SbsConfigEntry));
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_config_body(parser, configuration_info);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
            
            for (size_t i=0; i < fl_array_length(envs); i++)
            {
                char *env = envs[i];

                if (flm_cstring_equals(BASE_CONFIG_KEY, env))
                    flm_vexit(ERR_FATAL, BASE_CONFIG_KEY " is a reserved keyword and cannot be used as an environment name (in line %ld, column %ld)\n", token->line, token->col);

                fl_hashtable_add(configuration->entries, env, configuration_info);
            }

            fl_array_free_each(envs, sbs_common_free_string);
        }
        else
        {
            parse_config_body(parser, base_config_entry);
        }        

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return configuration;
}

static void find_ancestors(const struct SbsConfigSection *config, FlList ancestors, FlHashtable config_map, const char *env_name)
{
    if (!config->extends)
        return;

    size_t length = fl_array_length(config->extends);
    for (size_t i=0; i < length; i++)
    {
        struct SbsConfigSection *parent_config = fl_hashtable_get(config_map, config->extends[i]);

        if (fl_hashtable_has_key(parent_config->entries, env_name))
            fl_list_prepend(ancestors, fl_hashtable_get(parent_config->entries, env_name));    

        fl_list_prepend(ancestors, fl_hashtable_get(parent_config->entries, BASE_CONFIG_KEY));

    }
    
    for (size_t i=0; i < length; i++)
        find_ancestors(fl_hashtable_get(config_map, config->extends[i]), ancestors, config_map, env_name);
}

static void merge_compile_config(struct SbsConfigCompile *extend, const struct SbsConfigCompile *source)
{
    if (source->extension)
        extend->extension = source->extension;
    
    if (source->flags)
        sbs_common_extend_fl_array(&extend->flags, source->flags);

    if (source->include_dir_flag)
        extend->include_dir_flag = source->include_dir_flag;

    if (source->define_flag)
        extend->define_flag = source->define_flag;
}

static void merge_archive_config(struct SbsConfigArchive *extend, const struct SbsConfigArchive *source)
{
    if (source->extension)
        extend->extension = source->extension;

    if (source->flags)
        sbs_common_extend_fl_array(&extend->flags, source->flags);   
}

static void merge_shared_config(struct SbsConfigShared *extend, const struct SbsConfigShared *source)
{
    if (source->extension)
        extend->extension = source->extension;

    if (source->flags)
        sbs_common_extend_fl_array(&extend->flags, source->flags);   
}

static void merge_executable_config(struct SbsConfigExecutable *extend, const struct SbsConfigExecutable *source)
{
    if (source->extension)
        extend->extension = source->extension;

    if (source->flags)
        sbs_common_extend_fl_array(&extend->flags, source->flags);   
}

struct SbsConfiguration* sbs_config_resolve(const char *config_name, FlHashtable config_map, const char *env_name)
{    
    const struct SbsConfigSection *config = fl_hashtable_get(config_map, config_name);

    if (!config)
        return NULL;

    struct SbsConfiguration *extended_config = fl_malloc(sizeof(struct SbsConfiguration));
    extended_config->name = config->name;

    FlList hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(config->entries, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(config->entries, env_name));
    fl_list_prepend(hierarchy, fl_hashtable_get(config->entries, BASE_CONFIG_KEY));
    find_ancestors(config, hierarchy, config_map, env_name);

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsConfigEntry *ancestor = (const struct SbsConfigEntry*)node->value;

        // struct SbsConfigCompile compile;
        merge_compile_config(&extended_config->compile, &ancestor->compile);
        merge_archive_config(&extended_config->archive, &ancestor->archive);
        merge_shared_config(&extended_config->shared, &ancestor->shared);
        merge_executable_config(&extended_config->executable, &ancestor->executable);

        node = node->next;
    }

    fl_list_free(hierarchy);

    return extended_config;
}

void sbs_config_release(struct SbsConfiguration *extended_config)
{
    if (extended_config->compile.flags)
        fl_array_free(extended_config->compile.flags);

    if (extended_config->archive.flags)
        fl_array_free(extended_config->archive.flags);

    if (extended_config->shared.flags)
        fl_array_free(extended_config->shared.flags);

    if (extended_config->executable.flags)
        fl_array_free(extended_config->executable.flags);

    fl_free(extended_config);
}
