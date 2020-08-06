#include <stddef.h>
#include "conditional.h"
#include "expression.h"
#include "parser.h"
#include "variable.h"

SbsStmtConditional* sbs_stmt_conditional_parse(SbsParser *parser)
{
    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
    {
        sbs_parser_consume(parser, SBS_TOKEN_FOR);
    }
    else if (sbs_parser_peek(parser)->type == SBS_TOKEN_IF)
    {
        sbs_parser_consume(parser, SBS_TOKEN_IF);
    }
    else
    {
        return NULL;
    }

    SbsStmtConditional *conditional = sbs_stmt_conditional_new();
    conditional->expr = sbs_expression_parse(parser);

    return conditional;
}
