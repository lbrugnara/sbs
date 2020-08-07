#include <fllib.h>
#include "parser.h"
#include "helpers.h"

/*
 * Function: sbs_parse_identifier
 *  Returns an string that represents an identifier
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char* - Parsed string
 *
 */
char* sbs_parse_identifier(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    
    return fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
}

char** sbs_parse_identifier_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many strings
    size_t identifier_count = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        identifier_count++;
    }

    char **identifiers = NULL;

    if (identifier_count != 0)
    {
        // Parse the identifiers
        identifiers = fl_array_new(sizeof(char*), identifier_count);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            identifiers[index++] = sbs_parse_identifier(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return identifiers;
}

/*
 * Function: sbs_parse_extends_declaration
 *  Parses an *extends* declaration that expects multiple comma separated identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - List of identifiers referenced by the *extends* declaration
 *
 */
char** sbs_parse_extends_declaration(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_EXTENDS);

    // Track how many identifiers
    size_t identifiersCount = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type == SBS_TOKEN_COMMA || tmp->type == SBS_TOKEN_IDENTIFIER; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        identifiersCount++;
    }
    
    // Parse the identifiers
    char **identifiers = fl_array_new(sizeof(char*), identifiersCount);
    size_t index = 0;

    while (identifiersCount--)
    {
        const SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*)id->value.sequence, id->value.length);

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}
