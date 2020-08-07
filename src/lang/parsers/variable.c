#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include <fllib/Array.h>
#include "variable.h"
#include "parser.h"
#include "expression.h"
#include "helpers.h"
#include "cstring.h"

SbsNodeVariableDefinition* sbs_parse_variable_definition(SbsParser *parser)
{
    SbsNodeVariableDefinition *var_def = sbs_node_variable_definition_new();
    var_def->name = sbs_expression_variable_parse(parser);

    sbs_parser_consume(parser, SBS_TOKEN_ASSIGN);

    // TODO: Update this to support more types
    var_def->kind = SBS_VARIABLE_TYPE_STRING;
    var_def->value.s = sbs_cstring_parse(parser);

    return var_def;
}

SbsVariableExpr** sbs_parse_variable_array(SbsParser *parser)
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

    SbsVariableExpr **variables = NULL;

    if (var_count != 0)
    {
        // Parse the variables
        variables = fl_array_new(sizeof(SbsVariableExpr*), var_count);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            variables[index++] = sbs_expression_variable_parse(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return variables;
}
