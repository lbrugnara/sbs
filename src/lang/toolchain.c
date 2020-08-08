#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "toolchain.h"
#include "helpers.h"
#include "cstring.h"
#include "parser.h"
#include "expr.h"
#include "expr-if.h"
#include "../utils.h"

SbsSectionToolchain* sbs_section_toolchain_new(const struct FlSlice *name)
{
    SbsSectionToolchain *toolchain = fl_malloc(sizeof(SbsSectionToolchain));

    toolchain->name = sbs_slice_to_cstring(name);
    toolchain->entries = fl_array_new(sizeof(SbsNodeToolchain*), 0);

    return toolchain;
}

SbsNodeToolchain* sbs_section_toolchain_add_node(SbsSectionToolchain *toolchain_section)
{
    SbsNodeToolchain *toolchain_entry = fl_malloc(sizeof(SbsNodeToolchain));

    toolchain_section->entries = fl_array_append(toolchain_section->entries, &toolchain_entry);

    return toolchain_entry;
}

static void sbs_node_toolchain_free(SbsNodeToolchain *toolchain_entry)
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

    if (toolchain_entry->condition)
        sbs_expr_free(toolchain_entry->condition);
    
    fl_free(toolchain_entry);
}

void sbs_section_toolchain_free(SbsSectionToolchain *toolchain)
{
    fl_cstring_free(toolchain->name);

    if (toolchain->entries)
        fl_array_free_each_pointer(toolchain->entries, (FlArrayFreeElementFunc) sbs_node_toolchain_free);

    if (toolchain->condition)
        sbs_expr_free(toolchain->condition);

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
                    toolchain->compiler.bin = sbs_cstring_parse(parser);
                }
                else if (sbs_token_equals(token, "include_dir_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->compiler.include_dir_flag = sbs_cstring_parse(parser);
                }
                else if (sbs_token_equals(token, "define_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->compiler.define_flag = sbs_cstring_parse(parser);
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
                    toolchain->archiver.bin = sbs_cstring_parse(parser);
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
                    toolchain->linker.bin = sbs_cstring_parse(parser);
                }
                else if (sbs_token_equals(token, "lib_dir_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->linker.lib_dir_flag = sbs_cstring_parse(parser);
                }
                else if (sbs_token_equals(token, "lib_flag"))
                {
                    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                    sbs_parser_consume(parser, SBS_TOKEN_COLON);
                    toolchain->linker.lib_flag = sbs_cstring_parse(parser);
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
        toolchain->condition = sbs_expr_parse_for(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        SbsNodeToolchain *toolchain_entry = sbs_section_toolchain_add_node(toolchain);

        if (sbs_parser_peek(parser)->type == SBS_TOKEN_IF)
        {
            const SbsToken *token = sbs_parser_peek(parser);

            // Parse the for declaration
            toolchain_entry->condition = sbs_expr_parse_if(parser);
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
