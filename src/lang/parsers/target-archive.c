#include "target-archive.h"
#include "../target-archive.h"
#include "target.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "../../utils.h"

void sbs_section_archive_body_parse(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target)
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
            sbs_section_target_for_condition_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_SECTION_TARGET_ARCHIVE);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a target archive block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

SbsAbstractSectionTarget* sbs_section_archive_parse(SbsParser *parser)
{
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    SbsSectionArchive *target_section = sbs_section_archive_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    sbs_section_archive_body_parse(parser, target_section, sbs_section_archive_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}
