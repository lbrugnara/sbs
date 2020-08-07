#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "toolchain.h"
#include "helpers.h"
#include "parser.h"
#include "expression.h"
#include "../../utils.h"

static void parse_toolchain_entry(SbsParser *parser, SbsNodeToolchain *toolchain)
{
    while (sbs_parser_peek(parser)->type == SBS_TOKEN_IDENTIFIER)
    {
        const SbsToken *property = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        sbs_parser_consume(parser, SBS_TOKEN_COLON);
        sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

        if (sbs_token_equals(property, "compiler"))
        {
            const SbsToken *token = NULL;
            while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE)
            {
                if (sbs_token_equals(token, "bin"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->compiler.bin = sbs_parse_string(parser);
                }
                else if (sbs_token_equals(token, "include_dir_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->compiler.include_dir_flag = sbs_parse_string(parser);
                }
                else if (sbs_token_equals(token, "define_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->compiler.define_flag = sbs_parse_string(parser);
                }
                else
                {
                    sbs_parser_error(parser, property, "while parsing a toolchain's compiler block");
                }
                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }
        }
        else if (sbs_token_equals(property, "archiver"))
        {
            const SbsToken *token = NULL;
            while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE)
            {
                if (sbs_token_equals(token, "bin"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->archiver.bin = sbs_parse_string(parser);
                }
                else
                {
                    sbs_parser_error(parser, property, "while parsing a toolchain's archiver block");
                }
                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }
        }
        else if (sbs_token_equals(property, "linker"))
        {
            const SbsToken *token = NULL;
            while ((token = sbs_parser_peek(parser)) != NULL && token->type != SBS_TOKEN_RBRACE)
            {
                if (sbs_token_equals(token, "bin"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->linker.bin = sbs_parse_string(parser);
                }
                else if (sbs_token_equals(token, "lib_dir_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->linker.lib_dir_flag = sbs_parse_string(parser);
                }
                else if (sbs_token_equals(token, "lib_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->linker.lib_flag = sbs_parse_string(parser);
                }
                else
                {
                    sbs_parser_error(parser, property, "while parsing a toolchain's linker block");
                }
                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }
        }
        else
        {
            sbs_parser_error(parser, property, "while parsing a toolchain block");
        }

        sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_section_toolchain_parse
 *  Parses a *toolchain* block which supports the following properties:
 *      - compiler: The executable path to the compiler
 *      - archiver: The executable path to the archiver
 *      - linker: The executable path to the linker
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  static SbsSectionToolchain* - The parsed *toolchain* block
 *
 */
SbsSectionToolchain* sbs_section_toolchain_parse(SbsParser *parser)
{
    // Consume 'toolchain'
    sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    SbsSectionToolchain *toolchain = sbs_section_toolchain_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        toolchain->condition = sbs_statement_for_parse(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        SbsNodeToolchain *toolchain_entry = sbs_section_toolchain_add_node(toolchain);

        if (sbs_parser_peek(parser)->type == SBS_TOKEN_IF)
        {
            const SbsToken *token = sbs_parser_peek(parser);

            // Parse the for declaration
            toolchain_entry->condition = sbs_statement_if_parse(parser);
            // Parse the toolchain info
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_toolchain_entry(parser, toolchain_entry);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else
        {            
            parse_toolchain_entry(parser, toolchain_entry);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return toolchain;
}
