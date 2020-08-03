#include "target-compile.h"
#include "../target-compile.h"
#include "target.h"
#include "action.h"
#include "helpers.h"
#include "parser.h"
#include "../../utils.h"

void sbs_section_compile_body_parse(SbsParser *parser, SbsSectionCompile *target_section, SbsNodeCompile *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        
        if (sbs_token_equals(token, "includes"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->includes = sbs_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "sources"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->sources = sbs_parse_string_array(parser);
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
        else if (sbs_token_equals(token, "defines"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
        }
        else if (token->type == SBS_TOKEN_IF)
        {
            sbs_section_target_for_condition_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_COMPILE);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a target compile block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

SbsAbstractSectionTarget* sbs_section_compile_parse(SbsParser *parser)
{
    // Consume the 'compile' token
    sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    SbsSectionCompile *target_section = sbs_section_compile_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    sbs_section_compile_body_parse(parser, target_section, sbs_section_compile_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}