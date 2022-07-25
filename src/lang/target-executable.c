#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-executable.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "cstring.h"
#include "parser.h"
#include "../utils.h"

SbsSectionExecutable* sbs_section_executable_new(const struct FlSlice *name)
{
    SbsSectionExecutable *section = fl_malloc(sizeof(SbsSectionExecutable));

    section->base.type = SBS_SECTION_TARGET_EXECUTABLE;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeExecutable*), 0);

    return section;
}

SbsNodeExecutable* sbs_section_executable_add_node(SbsSectionExecutable *section)
{
    SbsNodeExecutable *node = fl_malloc(sizeof(SbsNodeExecutable));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

static void free_library_node(void *obj)
{
    if (!obj)
        return;

    SbsPropertyLibrary *lib = (SbsPropertyLibrary*)obj;

    if (lib->name)
        fl_cstring_free(lib->name);

    if (lib->path)
        fl_cstring_free(lib->path);
}

void sbs_node_executable_free(SbsNodeExecutable *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);

    if (node->objects)
        fl_array_free_each_pointer(node->objects, (FlArrayFreeElementFunc) sbs_source_free);

    if (node->libraries)
        fl_array_free_each(node->libraries, free_library_node);

    if (node->output_name)
        fl_cstring_free(node->output_name);

    if (node->defines)
        fl_array_free_each_pointer(node->defines, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(node);
}

void sbs_section_executable_free(SbsSectionExecutable *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_executable_free);

    fl_free(section);
}

static SbsPropertyLibrary* parse_library_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    SbsPropertyLibrary *libraries = fl_array_new(sizeof(SbsPropertyLibrary), 0);

    const SbsToken *token;
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET) {
        sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

        SbsPropertyLibrary library = { 0 };

        while ((token = sbs_parser_peek(parser))->type != SBS_TOKEN_RBRACE) {
            if (sbs_token_equals(token, "path")) {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.path = sbs_cstring_parse(parser);
            } else if (sbs_token_equals(token, "name")) {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.name = sbs_cstring_parse(parser);
            } else {
                sbs_parser_error(parser, token, "while parsing a target library node");
            }
        }

        sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

        libraries = fl_array_append(libraries, &library);
        
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return libraries;
}

void sbs_section_executable_body_parse(SbsParser *parser, SbsSectionExecutable *target_section, SbsNodeExecutable *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE) {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_source_array_parse(parser);
        } else if (sbs_token_equals(token, "output_name")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_cstring_parse(parser);
        } else if (sbs_token_equals(token, "output_dir")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_cstring_parse(parser);
        } else if (sbs_token_equals(token, "actions")) {
            target->base.actions = sbs_property_actions_parse(parser);
        } else if (sbs_token_equals(token, "libraries")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->libraries = parse_library_array(parser);
        } else if (sbs_token_equals(token, "defines")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_cstring_array_parse(parser);
        } else if (sbs_token_equals(token, "flags")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.flags = sbs_expr_parse_string_array(parser);
        } else if (token->type == SBS_TOKEN_IF) {
            sbs_section_target_if_stmt_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_SECTION_TARGET_EXECUTABLE);
        } else {
            sbs_parser_error(parser, token, "while parsing a target executable block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

SbsAbstractSectionTarget* sbs_section_executable_parse(SbsParser *parser)
{
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    SbsSectionExecutable *target_section = sbs_section_executable_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    sbs_section_executable_body_parse(parser, target_section, sbs_section_executable_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}
