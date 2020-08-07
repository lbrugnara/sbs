#include <fllib.h>
#include "parser.h"
#include "../command.h"
#include "string.h"
#include "helpers.h"

/*
 * Function: sbs_command_string_parse
 *  Returns an array of strings (that represents executable commands)
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings (that represents executable commands)
 *
 */
SbsString* sbs_command_string_parse(SbsParser *parser)
{
    return sbs_string_parse(parser);
}

/*
 * Function: sbs_command_array_parse
 *  Returns an array of strings (that represents executable commands) or ids
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings (that represents executable commands) or ids
 *
 */
SbsCommand** sbs_command_array_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many elements
    size_t nelements = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        nelements++;
    }

    SbsCommand **elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(SbsCommand*), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const SbsToken *element = sbs_parser_peek(parser);

            SbsCommand *command = NULL;

            if (element->type == SBS_TOKEN_COMMAND_STRING)
            {
                command = fl_malloc(sizeof(SbsCommand));
                command->type = SBS_COMMAND_STRING;
                command->value = sbs_command_string_parse(parser);
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                command = fl_malloc(sizeof(SbsCommand));
                command->type = SBS_COMMAND_NAME;
                command->value = sbs_string_new(sbs_parse_identifier(parser), true);
            }
            else
            {
                sbs_parser_error(parser, element, "while parsing an array of commands or IDs");
            }

            elements[index++] = command;

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}
