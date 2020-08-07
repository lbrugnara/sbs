#include <fllib.h>
#include "parser.h"
#include "../command.h"
#include "expression.h"
#include "helpers.h"

SbsCommand* sbs_command_parse(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    SbsCommand *command = NULL;
    if (token->type == SBS_TOKEN_COMMAND_STRING)
    {
        command = fl_malloc(sizeof(SbsCommand));
        command->type = SBS_COMMAND_STRING;
        command->value.str = sbs_expression_string_parse(parser);
    }
    else if (token->type == SBS_TOKEN_IDENTIFIER)
    {
        command = fl_malloc(sizeof(SbsCommand));
        command->type = SBS_COMMAND_NAME;
        command->value.id = sbs_expression_identifier_parse(parser);
    }
    else
    {
        sbs_parser_error(parser, token, "while parsing an action body");
    }

    return command;
}

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
            elements[index++] = sbs_command_parse(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}
