#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "configuration.h"
#include "helpers.h"
#include "string.h"
#include "parser.h"
#include "conditional.h"
#include "../../utils.h"

static void parse_compile_block(SbsParser *parser, SbsNodeConfigCompile *compile)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_token_equals(token, "flags"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->flags = sbs_value_string_array_parse(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            compile->extension = sbs_value_string_parse(parser);
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
            archive->flags = sbs_value_string_array_parse(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            archive->extension = sbs_value_string_parse(parser);
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
            shared->flags = sbs_value_string_array_parse(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            shared->extension = sbs_value_string_parse(parser);
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
            executable->flags = sbs_value_string_array_parse(parser);
        }
        else if (sbs_token_equals(token, "extension"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            executable->extension = sbs_value_string_parse(parser);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing an executable configuration section");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

static void parse_config_body(SbsParser *parser, struct SbsNodeConfig *configuration)
{
    const SbsToken *token = NULL;

    while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_IF)
    {
        if (sbs_token_equals(token, "compile"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_compile_block(parser, &configuration->compile);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "archive"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_archive_block(parser, &configuration->archive);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "shared"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_SHARED);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_shared_block(parser, &configuration->shared);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (sbs_token_equals(token, "executable"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_executable_block(parser, &configuration->executable);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
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
            configuration->condition = sbs_stmt_conditional_parse(parser);
    }
    else if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
    {
        configuration->condition = sbs_stmt_conditional_parse(parser);

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
            // Parse the for declaration
            config_node->condition = sbs_stmt_conditional_parse(parser);

            // Parse the configuration info
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_config_body(parser, config_node);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else
        {
            parse_config_body(parser, config_node);
        }        

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return configuration;
}
