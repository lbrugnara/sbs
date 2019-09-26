#include <fllib.h>
#include "configuration.h"
#include "common.h"
#include "parser.h"
#include "../common.h"

void sbs_config_entry_free(struct SbsConfigNode *config)
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


void sbs_config_section_free(struct SbsConfigSection *configuration)
{
    fl_cstring_free(configuration->name);

    if (configuration->extends)
        fl_array_free_each(configuration->extends, sbs_common_free_string);

    if (configuration->nodes)
    {
        // We share the same struct SbsConfigNode between multiple environments, so we nullify 
        // duplicates, release the objects, and finally release the memory used by the array returned
        // by fl_hashtable_values
        struct SbsConfigNode **configurations = fl_hashtable_values(configuration->nodes);

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
        fl_hashtable_free(configuration->nodes);
    }

    fl_free(configuration);
}

static void parse_compile_block(struct SbsParser *parser, struct SbsConfigCompileNode *compile)
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

static void parse_archive_block(struct SbsParser *parser, struct SbsConfigArchiveNode *archive)
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

static void parse_shared_block(struct SbsParser *parser, struct SbsConfigSharedNode *shared)
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

static void parse_executable_block(struct SbsParser *parser, struct SbsConfigExecutableNode *executable)
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

static void parse_config_body(struct SbsParser *parser, struct SbsConfigNode *configuration)
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
struct SbsConfigSection* sbs_config_section_parse(struct SbsParser *parser)
{
    struct SbsConfigSection *configuration = fl_malloc(sizeof(struct SbsConfigSection));

    configuration->nodes = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsConfigNode *base_config_entry = fl_malloc(sizeof(struct SbsConfigNode));

    // Use the special key #base for the default config
    fl_hashtable_add(configuration->nodes, SBS_BASE_OBJECT_KEY, base_config_entry);

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
            struct SbsConfigNode *configuration_info = fl_malloc(sizeof(struct SbsConfigNode));
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_config_body(parser, configuration_info);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
            
            for (size_t i=0; i < fl_array_length(envs); i++)
            {
                char *env = envs[i];

                if (flm_cstring_equals(SBS_BASE_OBJECT_KEY, env))
                    flm_vexit(ERR_FATAL, SBS_BASE_OBJECT_KEY " is a reserved keyword and cannot be used as an environment name (in line %ld, column %ld)\n", token->line, token->col);

                fl_hashtable_add(configuration->nodes, env, configuration_info);
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
