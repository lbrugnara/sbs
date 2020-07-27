#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include <fllib/Array.h>
#include "parser.h"
#include "variable.h"

SbsValueVariable* sbs_parse_variable(SbsParser *parser)
{
    const SbsToken *var_token = sbs_parser_consume(parser, SBS_TOKEN_VARIABLE);
    
    SbsValueVariable *variable = NULL;

    for (size_t i = var_token->value.length - 1; true; )
    {
        if (((const char*) var_token->value.sequence)[i] == '.')
        {
            const struct FlSlice name = fl_slice_new(var_token->value.sequence, 1, i + 1, var_token->value.length - (i + 1));
            const struct FlSlice namespace = fl_slice_new(var_token->value.sequence, 1, 0, i);
            variable = sbs_value_variable_new(&name, &namespace);
            break;
        }
        else if (i == 0)
        {
            variable = sbs_value_variable_new(&var_token->value, NULL);
            break;
        }
        i--;
    }

    return variable;
}

SbsValueVariable** sbs_parse_variable_array(SbsParser *parser)
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

    SbsValueVariable **variables = NULL;

    if (var_count != 0)
    {
        // Parse the variables
        variables = fl_array_new(sizeof(SbsValueVariable*), var_count);
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
