#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-if.h"
#include "expr-binary.h"

void sbs_expr_free_if(SbsIfExpr *if_node)
{
    sbs_expr_free(if_node->condition);
    sbs_expr_free(if_node->then_branch);
    sbs_expr_free(if_node->else_branch);
    fl_free(if_node);
}

SbsIfExpr* sbs_expr_copy_if(SbsIfExpr *if_expr)
{
    SbsIfExpr *copy = fl_malloc(sizeof(SbsIfExpr));

    copy->kind = SBS_EXPR_IF;
    copy->condition = sbs_expr_copy(if_expr->condition);
    copy->then_branch = sbs_expr_copy(if_expr->then_branch);
    copy->else_branch = sbs_expr_copy(if_expr->else_branch);

    return copy;
}

SbsIfExpr* sbs_expr_make_if(SbsExpression *condition, SbsExpression *then_branch, SbsExpression *else_branch)
{
    SbsIfExpr *if_expr = fl_malloc(sizeof(SbsIfExpr));

    if_expr->kind = SBS_EXPR_IF;
    if_expr->condition = condition;
    if_expr->then_branch = then_branch;
    if_expr->else_branch = else_branch;

    return if_expr;
}

SbsExpression* sbs_expr_parse_conditional(SbsParser *parser)
{
    SbsExpression *expr = sbs_expr_parse_or(parser);

    if (sbs_parser_next_is(parser, SBS_TOKEN_QUESTION))
    {
        sbs_parser_consume(parser, SBS_TOKEN_QUESTION);
        SbsExpression *then_branch = sbs_expr_parse(parser);
        sbs_parser_consume(parser, SBS_TOKEN_COLON);
        SbsExpression *else_branch = sbs_expr_parse(parser);

        expr = (SbsExpression*) sbs_expr_make_if(expr, then_branch, else_branch);
    }

    return expr;
}

SbsExpression* sbs_expr_parse_if(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_IF);
    return sbs_expr_parse(parser);
}
