#include "preset.h"
#include "helpers.h"
#include "parser.h"

void sbs_section_preset_free(SbsSectionPreset *preset)
{
    fl_cstring_free(preset->name);

    if (preset->env)
        fl_array_free_each_pointer(preset->env, (FlArrayFreeElementFunc) fl_cstring_free);

    if (preset->toolchain)
        fl_cstring_free(preset->toolchain);

    if (preset->config)
        fl_cstring_free(preset->config);

    if (preset->target)
        fl_cstring_free(preset->target);

    sbs_property_actions_free(&preset->actions);

    fl_free(preset);
}

/*
 * Function: sbs_section_preset_parse
 *  Parses a *preset* block which supports the following properties:
 *      - env: Environment name
 *      - toolchain: Toolchain name
 *      - config: Configuration name
 *      - target: Target name
 *      - actions: Actions to run before and after the build process
 * 
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsSectionPreset* - Parsed *preset* block
 *
 */
SbsSectionPreset* sbs_section_preset_parse(SbsParser *parser)
{
    SbsSectionPreset *preset = fl_malloc(sizeof(SbsSectionPreset));

    // Consume the 'preset' token
    sbs_parser_consume(parser, SBS_TOKEN_PRESET);

    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    preset->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    const SbsToken *token = NULL;

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (token->type == SBS_TOKEN_ENV)
        {
            sbs_parser_consume(parser, SBS_TOKEN_ENV);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);

            if (sbs_parser_peek(parser)->type == SBS_TOKEN_LBRACKET)
            {
                preset->env = sbs_parse_identifier_array(parser);
            }
            else
            {
                preset->env = fl_array_new(sizeof(char**), 1);
                preset->env[0] = sbs_parse_identifier(parser);
            }            
        }
        else if (token->type == SBS_TOKEN_TOOLCHAIN)
        {
            sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->toolchain = sbs_parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_CONFIG)
        {
            sbs_parser_consume(parser, SBS_TOKEN_CONFIG);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->config = sbs_parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_TARGET)
        {
            sbs_parser_consume(parser, SBS_TOKEN_TARGET);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->target = sbs_parse_identifier(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            preset->actions = sbs_property_actions_parse(parser);
        }
        else
        {
            sbs_parser_warning(parser, token, "is an unrecognized token");
            sbs_parser_sync(parser, (SbsTokenType[]) { SBS_TOKEN_COMMA, SBS_TOKEN_RBRACE }, 2);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume_if(parser, SBS_TOKEN_RBRACE);

    return preset;
}
