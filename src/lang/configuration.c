#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"
#include "parser.h"
#include "expr.h"
#include "expr-if.h"
#include "expr-binary.h"
#include "expr-string.h"
#include "helpers.h"
#include "../utils.h"

static void sbs_config_entry_free(struct SbsNodeConfig *config)
{
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

    if (config->condition)
        sbs_expr_free(config->condition);

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
        sbs_expr_free(configuration->condition);

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


static void parse_compile_block(SbsParser *parser, SbsNodeConfigCompile *compile)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "flags"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->flags = sbs_expr_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->extension = sbs_expr_parse_string(parser);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a compile configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_archive_block(SbsParser *parser, SbsNodeConfigArchive *archive)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "flags"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            archive->flags = sbs_expr_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            archive->extension = sbs_expr_parse_string(parser);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing an archive configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_shared_block(SbsParser *parser, SbsNodeConfigShared *shared)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "flags"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            shared->flags = sbs_expr_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            shared->extension = sbs_expr_parse_string(parser);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a shared configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_executable_block(SbsParser *parser, SbsNodeConfigExecutable *executable)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "flags"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            executable->flags = sbs_expr_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            executable->extension = sbs_expr_parse_string(parser);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing an executable configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_config_body(SbsParser *parser, SbsSectionConfig *config_section, struct SbsNodeConfig *config_node)
{
    const SbsToken *token = NULL;

    while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "compile"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_compile_block(parser, &config_node->compile);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "archive"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_archive_block(parser, &config_node->archive);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "shared"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_SHARED);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_shared_block(parser, &config_node->shared);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "executable"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_executable_block(parser, &config_node->executable);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (token->type == SBS_TOKEN_IF)
        {
            struct SbsNodeConfig *nested_config_node = sbs_section_config_add_node(config_section);
            nested_config_node->condition = sbs_expr_parse_if(parser);

            // NOTE: This allows nested ifs but without too much effort, because of that we need
            // to "merge" the condition of the nested if with the one in its parent node
            // TODO: If we implement proper nested ifs, we need to update this
            if (config_node->condition != NULL)
            {
                SbsBinaryExpr *merged_expressions = sbs_expr_make_binary(SBS_EXPR_OP_AND, 
                                                                                sbs_expr_copy(config_node->condition), 
                                                                                nested_config_node->condition);
                nested_config_node->condition = (SbsExpression*) merged_expressions;
            }

            // Parse the configuration info
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_config_body(parser, config_section, nested_config_node);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

            if (sbs_parser_next_is(parser, SBS_TOKEN_ELSE))
            {
                // NOTE: We consume the else and check for the next token:
                //  - SBS_TOKEN_IF: We iterate one more time "to fall" in the current else-if that will "merge"
                //    the config_node object with a new nested_config_node object.
                //  - any other: we call 'parse_config_body' with the same config_node object
                // TODO: If we implement proper if-else, we need to update this
                sbs_parser_consume(parser, SBS_TOKEN_ELSE);

                if (sbs_parser_next_is(parser, SBS_TOKEN_IF))
                    continue;

                sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
                parse_config_body(parser, config_section, config_node);
                sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
            }
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a configuration section");
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
 *  SbsSectionConfig* - The parsed *config* block
 *
 */
SbsSectionConfig* sbs_section_config_parse(SbsParser *parser)
{
    // Consume 'configuration'
    sbs_parser_consume(parser, SBS_TOKEN_CONFIG);
    
    // Consume IDENTIFIER
    SbsSectionConfig *configuration = sbs_section_config_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    // TODO: Improve this:
    if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
    {
        configuration->extends = sbs_parse_extends_declaration(parser);

        if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
            configuration->condition = sbs_expr_parse_for(parser);
    }
    else if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
    {
        configuration->condition = sbs_expr_parse_for(parser);

        if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
            configuration->extends = sbs_parse_extends_declaration(parser);
    }

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        struct SbsNodeConfig *config_node = sbs_section_config_add_node(configuration);

        if (token->type == SBS_TOKEN_IF)
        {
            // Parse the if declaration
            config_node->condition = sbs_expr_parse_if(parser);

            // Parse the configuration info
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_config_body(parser, configuration, config_node);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

            if (sbs_parser_next_is(parser, SBS_TOKEN_ELSE))
            {
                // NOTE: We consume the "else" and we let the next iteration to trigger the if
                // in the case of an "else-if" or the "parse_config_body" if it is just an "else"
                // TODO: If we implement proper if-else, we need to update this
                sbs_parser_consume(parser, SBS_TOKEN_ELSE);
            }
        }
        else
        {
            parse_config_body(parser, configuration, config_node);
        }        

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return configuration;
}
