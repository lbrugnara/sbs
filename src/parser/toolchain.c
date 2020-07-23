#include "toolchain.h"
#include "helpers.h"
#include "parser.h"
#include "../utils.h"

void sbs_toolchain_entry_free(SbsNodeToolchain *toolchain_entry)
{
    if (toolchain_entry->compiler.bin)
        fl_cstring_free(toolchain_entry->compiler.bin);

    if (toolchain_entry->compiler.define_flag)
        fl_cstring_free(toolchain_entry->compiler.define_flag);

    if (toolchain_entry->compiler.include_dir_flag)
        fl_cstring_free(toolchain_entry->compiler.include_dir_flag);

    if (toolchain_entry->archiver.bin)
        fl_cstring_free(toolchain_entry->archiver.bin);

    if (toolchain_entry->linker.bin)
        fl_cstring_free(toolchain_entry->linker.bin);

    if (toolchain_entry->linker.lib_dir_flag)
        fl_cstring_free(toolchain_entry->linker.lib_dir_flag);

    if (toolchain_entry->linker.lib_flag)
        fl_cstring_free(toolchain_entry->linker.lib_flag);

    if (toolchain_entry->for_clause)
        sbs_section_for_free(toolchain_entry->for_clause);
    
    fl_free(toolchain_entry);
}

void sbs_section_toolchain_free(SbsSectionToolchain *toolchain)
{
    fl_cstring_free(toolchain->name);

    if (toolchain->entries)
        fl_array_free_each_pointer(toolchain->entries, (FlArrayFreeElementFunc) sbs_toolchain_entry_free);

    if (toolchain->for_clause)
        sbs_section_for_free(toolchain->for_clause);

    fl_free(toolchain);
}

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
                    sbs_parser_error(property, "while parsing a toolchain's compiler block");
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
                    sbs_parser_error(property, "while parsing a toolchain's archiver block");
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
                    sbs_parser_error(property, "while parsing a toolchain's linker block");
                }
                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }
        }
        else
        {
            sbs_parser_error(property, "while parsing a toolchain block");
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
    SbsSectionToolchain *toolchain = fl_malloc(sizeof(SbsSectionToolchain));

    toolchain->entries = fl_array_new(sizeof(SbsNodeToolchain*), 0);

    // Consume 'toolchain'
    sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    toolchain->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        toolchain->for_clause = sbs_section_for_parse(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        SbsNodeToolchain *toolchain_entry = fl_malloc(sizeof(SbsNodeToolchain));
        toolchain->entries = fl_array_append(toolchain->entries, &toolchain_entry);
        if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        {
            const SbsToken *token = sbs_parser_peek(parser);

            // Parse the for declaration
            toolchain_entry->for_clause = sbs_section_for_parse(parser);
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
