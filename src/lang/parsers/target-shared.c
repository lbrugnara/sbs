#include "target-shared.h"
#include "../target-shared.h"
#include "target.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "../../utils.h"

void sbs_section_shared_body_parse(SbsParser *parser, SbsSectionShared *target_section, SbsNodeShared *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_parse(parser);
        }
        else if (sbs_token_equals(token, "output_name"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (token->type == SBS_TOKEN_IF)
        {
            sbs_section_target_if_stmt_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_SECTION_TARGET_SHARED);
        }
        else
        {
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
