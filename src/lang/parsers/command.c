#include <fllib.h>
#include "parser.h"
#include "command.h"
#include "helpers.h"

/*
 * Function: sbs_value_command_array_parse
 *  Returns an array of strings (that represents executable commands) or ids
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings (that represents executable commands) or ids
 *
 */
SbsValueCommand* sbs_value_command_array_parse(SbsParser *parser)
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

    SbsValueCommand *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(SbsValueCommand), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const SbsToken *element = sbs_parser_peek(parser);

            if (element->type == SBS_TOKEN_COMMAND_STRING)
            {
                elements[index++] = (SbsValueCommand) {
                    .type = SBS_COMMAND_STRING, 
                    .value = sbs_parse_command_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (SbsValueCommand) {
                    .type = SBS_COMMAND_NAME,
                    .value = sbs_parse_identifier(parser)
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