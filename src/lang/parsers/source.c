#include <fllib.h>
#include "parser.h"
#include "source.h"
#include "helpers.h"

/*
 * Function: sbs_value_source_array_prase
 *  Returns an array of strings or identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings or identifiers
 *
 */
SbsValueSource* sbs_value_source_array_prase(SbsParser *parser)
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

    SbsValueSource *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(SbsValueSource), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const SbsToken *element = sbs_parser_peek(parser);

            if (element->type == SBS_TOKEN_STRING)
            {
                elements[index++] = (SbsValueSource) {
                    .type = SBS_SOURCE_STRING, 
                    .value = sbs_parse_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (SbsValueSource) {
                    .type = SBS_SOURCE_NAME,
                    .value = sbs_parse_identifier(parser)
                };
            }
            else
            {
                sbs_parser_error(parser, element, "while parsing an array of strings or IDs");
            }

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}
