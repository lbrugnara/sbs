#include <fllib.h>
#include "parser.h"
#include "variable.h"

SbsVariable* sbs_parse_variable(SbsParser *parser)
{
    const SbsToken *var_token = sbs_parser_consume(parser, SBS_TOKEN_VARIABLE);
    
    const char *namespace = NULL;
    const char *name = NULL;

    for (size_t i = var_token->value.length - 1; true; )
    {
        if (((const char*) var_token->value.sequence)[i] == '.')
        {
            name = fl_cstring_dup_n((const char*) var_token->value.sequence + i + 1, var_token->value.length - (i + 1));
            namespace = fl_cstring_dup_n((const char*) var_token->value.sequence, i);
            break;
        }
        else if (i == 0)
        {
            name = fl_cstring_dup_n((const char*) var_token->value.sequence, var_token->value.length);
            break;
        }
        i--;
    }

    if (name == NULL)
        return NULL;
    
    SbsVariable *variable = fl_malloc(sizeof(SbsVariable));
    variable->namespace = namespace;
    variable->name = name;
    return variable;
}

SbsVariable** sbs_parse_variable_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many variables
    size_t var_count = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        var_count++;
    }

    SbsVariable **variables = NULL;

    if (var_count != 0)
    {
        // Parse the variables
        variables = fl_array_new(sizeof(SbsVariable*), var_count);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            variables[index++] = sbs_parse_variable(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return variables;
}
