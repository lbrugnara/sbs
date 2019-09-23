#include <fllib.h>
#include "configuration.h"
#include "common.h"
#include "parser.h"

void sbs_config_free(struct SbsConfiguration *config)
{
    fl_cstring_delete(config->name);

    if (config->extends)
        fl_array_delete_each(config->extends, sbs_common_free_string);

    if (config->for_toolchains)
        fl_array_delete_each(config->for_toolchains, sbs_common_free_string);


    if (config->compile.flags)
        fl_array_delete_each(config->compile.flags, sbs_common_free_string);

    if (config->compile.include_dir_flag)
        fl_cstring_delete(config->compile.include_dir_flag);

    if (config->compile.define_flag)
        fl_cstring_delete(config->compile.define_flag);

    if (config->compile.extension)
        fl_cstring_delete(config->compile.extension);


    if (config->archive.flags)
        fl_array_delete_each(config->archive.flags, sbs_common_free_string);

    if (config->archive.extension)
        fl_cstring_delete(config->archive.extension);


    if (config->shared.flags)
        fl_array_delete_each(config->shared.flags, sbs_common_free_string);
    
    if (config->shared.extension)
        fl_cstring_delete(config->shared.extension);


    if (config->executable.flags)
        fl_array_delete_each(config->executable.flags, sbs_common_free_string);

    if (config->executable.extension)
        fl_cstring_delete(config->executable.extension);


    fl_free(config);
}

static void free_map_entry(void *value)
{
    sbs_config_free((struct SbsConfiguration*)value);
}

void sbs_config_map_init(FlHashtable *config_map)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_map_entry
    };
    
    *config_map = fl_hashtable_new_args(new_args);
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
 *  struct SbsConfiguration* - The parsed *config* block
 *
 */
struct SbsConfiguration* sbs_config_parse(struct SbsParser *parser)
{
    struct SbsConfiguration *configuration = fl_malloc(sizeof(struct SbsConfiguration));

    // Consume 'configuration'
    sbs_parser_consume(parser, SBS_TOKEN_CONFIG);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    configuration->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
        configuration->extends = sbs_common_parse_extends_declaration(parser);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        configuration->for_toolchains = sbs_common_parse_for_declaration(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = sbs_parser_peek(parser);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"compile", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.flags = sbs_common_parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"include_dir_flag", 16))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.include_dir_flag = sbs_common_parse_string(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"define_flag", 11))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.define_flag = sbs_common_parse_string(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.extension = sbs_common_parse_string(parser);
                }
                else
                {
                    sbs_parser_error(token, "while parsing a configuration's compile block");
                }

                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }

            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"archive", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->archive.flags = sbs_common_parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->archive.extension = sbs_common_parse_string(parser);
                }

                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }

            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"shared", 6))
        {
            sbs_parser_consume(parser, SBS_TOKEN_SHARED);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->shared.flags = sbs_common_parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->shared.extension = sbs_common_parse_string(parser);
                }

                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }

            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"executable", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->executable.flags = sbs_common_parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    configuration->executable.extension = sbs_common_parse_string(parser);
                }

                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }

            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return configuration;
}


static void find_ancestors(const struct SbsConfiguration *config, FlList ancestors, FlHashtable config_map)
{
    if (!config->extends)
        return;

    size_t length = fl_array_length(config->extends);
    for (size_t i=0; i < length; i++)
        fl_list_prepend(ancestors, fl_hashtable_get(config_map, config->extends[i]));
    
    for (size_t i=0; i < length; i++)
        find_ancestors(fl_hashtable_get(config_map, config->extends[i]), ancestors, config_map);
}

bool sbs_config_inheritance_resolve(struct SbsConfiguration *extended_config, const char *config_name, const FlHashtable config_map)
{
    const struct SbsConfiguration *config = fl_hashtable_get(config_map, config_name);

    if (!config)
        return false;

    // Name will be always the targeted config
    extended_config->name = config->name;

    FlList ancestors = fl_list_new();

    // Using prepend we will keep the list ordered
    fl_list_prepend(ancestors, config);
    find_ancestors(config, ancestors, config_map);

    struct FlListNode *node = fl_list_head(ancestors);
    while (node)
    {
        const struct SbsConfiguration *ancestor = (const struct SbsConfiguration*)node->value;

        // char **for_toolchains;
        if (ancestor->for_toolchains)
            sbs_common_extend_fl_array(&(extended_config->for_toolchains), ancestor->for_toolchains);

        // struct SbsConfigCompile compile;
        if (ancestor->compile.extension)
            extended_config->compile.extension = ancestor->compile.extension;
        
        if (ancestor->compile.flags)
            sbs_common_extend_fl_array(&(extended_config->compile.flags), ancestor->compile.flags);

        if (ancestor->compile.include_dir_flag)
            extended_config->compile.include_dir_flag = ancestor->compile.include_dir_flag;

        if (ancestor->compile.define_flag)
            extended_config->compile.define_flag = ancestor->compile.define_flag;

        // struct SbsConfigArchive archive;
        if (ancestor->archive.extension)
            extended_config->archive.extension = ancestor->archive.extension;

        if (ancestor->archive.flags)
            sbs_common_extend_fl_array(&(extended_config->archive.flags), ancestor->archive.flags);

        // struct SbsConfigShared shared;
        if (ancestor->shared.extension)
            extended_config->shared.extension = ancestor->shared.extension;

        if (ancestor->shared.flags)
            sbs_common_extend_fl_array(&(extended_config->shared.flags), ancestor->shared.flags);

        // struct SbsConfigExecutable executable;
        if (ancestor->executable.extension)
            extended_config->executable.extension = ancestor->executable.extension;

        if (ancestor->executable.flags)
            sbs_common_extend_fl_array(&(extended_config->executable.flags), ancestor->executable.flags);

        node = node->next;
    }

    fl_list_delete(ancestors);

    return true;
}

void sbs_config_inheritance_clean(struct SbsConfiguration *extended_config)
{
    // char **for_toolchains;
    if (extended_config->for_toolchains)
        fl_array_delete(extended_config->for_toolchains);

    if (extended_config->compile.flags)
        fl_array_delete(extended_config->compile.flags);

    if (extended_config->compile.define_flag)
        fl_cstring_delete(extended_config->compile.define_flag);

    if (extended_config->archive.flags)
        fl_array_delete(extended_config->archive.flags);

    if (extended_config->shared.flags)
        fl_array_delete(extended_config->shared.flags);

    if (extended_config->executable.flags)
        fl_array_delete(extended_config->executable.flags);
}
