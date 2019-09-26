#include <fllib.h>
#include "common.h"
#include "parser.h"

/*
 * Function: sbs_common_parse_string
 *  Parses an string
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char* - Parsed string
 *
 */
char* sbs_common_parse_string(struct SbsParser *parser)
{
    const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_STRING);
    
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
 * Function: sbs_common_parse_command_string
 *  Returns an array of strings (that represents executable commands)
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings (that represents executable commands)
 *
 */
char* sbs_common_parse_command_string(struct SbsParser *parser)
{
    const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_COMMAND_STRING);
    
    char *string = fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
    if (fl_cstring_contains(string, "\\`"))
    {
        char *tmp = fl_cstring_replace(string, "\\`", "`");
        if (!tmp)
            return NULL;
        fl_cstring_free(string);
        string = tmp;
    }

    return string;
}


/*
 * Function: sbs_common_parse_identifier
 *  Returns an string that represents an identifier
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char* - Parsed string
 *
 */
char* sbs_common_parse_identifier(struct SbsParser *parser)
{
    const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    
    return fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
}

/*
 * Function: sbs_common_parse_command_string_or_id_array
 *  Returns an array of strings (that represents executable commands) or ids
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings (that represents executable commands) or ids
 *
 */
struct SbsStringOrId* sbs_common_parse_command_string_or_id_array(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many elements
    size_t nelements = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        nelements++;
    }

    struct SbsStringOrId *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(struct SbsStringOrId), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const struct SbsToken *element = sbs_parser_peek(parser);

            if (element->type == SBS_TOKEN_COMMAND_STRING)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_STRING, 
                    .value = sbs_common_parse_command_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_IDENTIFIER,
                    .value = sbs_common_parse_identifier(parser)
                };
            }
            else
            {
                sbs_parser_error(element, "while parsing an array of commands or IDs");
            }

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}


/*
 * Function: sbs_common_parse_string_or_id_array
 *  Returns an array of strings or identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings or identifiers
 *
 */
struct SbsStringOrId* sbs_common_parse_string_or_id_array(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many elements
    size_t nelements = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        nelements++;
    }

    struct SbsStringOrId *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(struct SbsStringOrId), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const struct SbsToken *element = sbs_parser_peek(parser);

            if (element->type == SBS_TOKEN_STRING)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_STRING, 
                    .value = sbs_common_parse_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_IDENTIFIER,
                    .value = sbs_common_parse_identifier(parser)
                };
            }
            else
            {
                sbs_parser_error(element, "while parsing an array of strings or IDs");
            }

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}

/*
 * Function: sbs_common_parse_string_array
 *  Returns an array of strings
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings
 *
 */
char** sbs_common_parse_string_array(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many strings
    size_t stringsCount = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        stringsCount++;
    }

    char **strings = NULL;

    if (stringsCount != 0)
    {
        // Parse the strings
        strings = fl_array_new(sizeof(char*), stringsCount);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            strings[index++] = sbs_common_parse_string(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return strings;
}


/*
 * Function: sbs_common_parse_extends_declaration
 *  Parses an *extends* declaration that expects multiple comma separated identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - List of identifiers referenced by the *extends* declaration
 *
 */
char** sbs_common_parse_extends_declaration(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_EXTENDS);

    // Track how many identifiers
    size_t identifiersCount = 0;
    const struct SbsToken *tmp;

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
        const struct SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*)id->value.sequence, id->value.length);

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}

/*
 * Function: sbs_common_parse_for_declaration
 *  Parses a *for* declaration that expects multiple comma separated identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - List of identifiers referenced by the *for* declaration
 *
 */
char** sbs_common_parse_for_declaration(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_FOR);

    // Track how many identifiers
    size_t identifiersCount = 0;
    const struct SbsToken *tmp;

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
        const struct SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*)id->value.sequence, id->value.length);

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}
