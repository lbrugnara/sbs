#include <stdlib.h>
#include <fllib/Cstring.h>
#include "command.h"
#include "expr.h"
#include "helpers.h"

void sbs_command_free(SbsCommand *command)
{
    if (command->type == SBS_COMMAND_STRING && command->value.str != NULL)
    {
        sbs_expr_free((SbsExpression*) command->value.str);
    }
    else if (command->type == SBS_COMMAND_NAME && command->value.id != NULL)
    {
        sbs_expr_free((SbsExpression*) command->value.id);
    }

    fl_free(command);
}

SbsCommand* sbs_command_copy(const SbsCommand *source)
{
    if (!source)
        return NULL;

    SbsCommand *copy = fl_malloc(sizeof(SbsCommand));

    copy->type = source->type;

    if (source->type == SBS_COMMAND_STRING)
    {
        copy->value.str = source->value.str != NULL ? (SbsStringExpr*) sbs_expr_copy((SbsExpression*) source->value.str) : NULL;
    }
    else if (source->type == SBS_COMMAND_NAME)
    {
        copy->value.id = source->value.id != NULL ? (SbsIdentifierExpr*) sbs_expr_copy((SbsExpression*) source->value.id) : NULL;
    }

    return copy;
}

SbsCommand* sbs_command_parse(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    SbsCommand *command = NULL;
    if (token->type == SBS_TOKEN_COMMAND_STRING)
    {
        command = fl_malloc(sizeof(SbsCommand));
        command->type = SBS_COMMAND_STRING;
        command->value.str = sbs_expr_parse_string(parser);
    }
    else if (token->type == SBS_TOKEN_IDENTIFIER)
    {
        command = fl_malloc(sizeof(SbsCommand));
        command->type = SBS_COMMAND_NAME;
        command->value.id = sbs_expr_parse_identifier(parser);
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
