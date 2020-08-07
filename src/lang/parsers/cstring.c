#include <fllib.h>
#include "parser.h"
#include "helpers.h"

/*
 * Function: sbs_cstring_parse
 *  Parses an string
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char* - Parsed string
 *
 */
char* sbs_cstring_parse(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_STRING);
    
    char *string = fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
    if (fl_cstring_contains(string, "\\\""))
    {
        char *tmp = fl_cstring_replace(string, "\\\"", "\"");
        if (!tmp)
            return NULL;
        fl_cstring_free(string);
        string = tmp;
    }

    return string;
}

/*
 * Function: sbs_cstring_array_parse
 *  Returns an array of strings
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings
 *
 */
char** sbs_cstring_array_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many strings
    size_t strings_count = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        strings_count++;
    }

    char **strings = NULL;

    if (strings_count != 0)
    {
        // Parse the strings
        strings = fl_array_new(sizeof(char*), strings_count);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            strings[index++] = sbs_cstring_parse(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return strings;
}
