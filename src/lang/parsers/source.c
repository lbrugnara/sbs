#include <fllib.h>
#include "parser.h"
#include "source.h"
#include "helpers.h"
#include "cstring.h"

/*
 * Function: sbs_source_array_parse
 *  Returns an array of strings or identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings or identifiers
 *
 */
SbsSource** sbs_source_array_parse(SbsParser *parser)
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

    SbsSource **elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(SbsSource*), nelements);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const SbsToken *element = sbs_parser_peek(parser);

            SbsSource *source = NULL;

            if (element->type == SBS_TOKEN_STRING)
            {
                source = fl_malloc(sizeof(SbsSource));
                source->type = SBS_SOURCE_STRING;
                source->value = sbs_cstring_parse(parser);
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                source = fl_malloc(sizeof(SbsSource));
                source->type = SBS_SOURCE_NAME;
                source->value = sbs_parse_identifier(parser);
            }
            else
            {
                sbs_parser_error(parser, element, "while parsing an array of strings or IDs");
            }

            elements[index++] = source;

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}
