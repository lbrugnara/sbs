#include <fllib/Array.h>
#include "target-executable.h"
#include "../target-executable.h"
#include "target.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "../../utils.h"

static SbsPropertyLibrary* parse_library_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    SbsPropertyLibrary *libraries = fl_array_new(sizeof(SbsPropertyLibrary), 0);

    const SbsToken *token;
    while ((token = sbs_parser_peek(parser))->type != SBS_TOKEN_RBRACKET)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

        SbsPropertyLibrary library = { 0 };

        while ((token = sbs_parser_peek(parser))->type != SBS_TOKEN_RBRACE)
        {
            if (sbs_token_equals(token, "path"))
            {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.path = sbs_parse_string(parser);
            }
            else if (sbs_token_equals(token, "name"))
            {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.name = sbs_parse_string(parser);
            }
            else
            {
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
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
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
        else if (sbs_token_equals(token, "libraries"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->libraries = parse_library_array(parser);
        }
        else if (sbs_token_equals(token, "defines"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
        }
        else if (token->type == SBS_TOKEN_IF)
        {
            sbs_section_target_for_condition_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_EXECUTABLE);
        }
        else
        {
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
