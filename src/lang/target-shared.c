#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-shared.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "cstring.h"
#include "parser.h"
#include "../utils.h"

SbsSectionShared* sbs_section_shared_new(const struct FlSlice *name)
{
    SbsSectionShared *section = fl_malloc(sizeof(SbsSectionShared));

    section->base.type = SBS_SECTION_TARGET_SHARED;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeShared*), 0);

    return section;
}

SbsNodeShared* sbs_section_shared_add_node(SbsSectionShared *section)
{
    SbsNodeShared *node = fl_malloc(sizeof(SbsNodeShared));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

void sbs_node_shared_free(SbsNodeShared *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);
    
    if (node->objects)
        fl_array_free_each_pointer(node->objects, (FlArrayFreeElementFunc) sbs_source_free);

    if (node->output_name)
        fl_cstring_free(node->output_name);

    fl_free(node);
}

void sbs_section_shared_free(SbsSectionShared *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_shared_free);

    fl_free(section);
}

void sbs_section_shared_body_parse(SbsParser *parser, SbsSectionShared *target_section, SbsNodeShared *target)
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
        } else if (sbs_token_equals(token, "flags")) {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.flags = sbs_expr_parse_string_array(parser);
        } else if (token->type == SBS_TOKEN_IF) {
            sbs_section_target_if_stmt_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_SECTION_TARGET_SHARED);
        } else {
            sbs_parser_error(parser, token, "while parsing a target shared block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_target_parse_shared
 *  Parses a *shared* target block which supports the following properties:
 *      - objects: List of object files to pass to the linker
 *      - output_name: Target's output name
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the link process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsAbstractSectionTarget* - Parsed *shared* target
 *
 */
SbsAbstractSectionTarget* sbs_section_shared_parse(SbsParser *parser)
{
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    SbsSectionShared *target_section = sbs_section_shared_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    sbs_section_shared_body_parse(parser, target_section, sbs_section_shared_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}
