#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-binary.h"
#include "expr-unary.h"

static SbsExprOperator BinaryOperators[] = {
    SBS_EXPR_OP_OR,
    SBS_EXPR_OP_AND,
    SBS_EXPR_OP_EQ,
    SBS_EXPR_OP_NEQ,
    SBS_EXPR_OP_IN,
};

void sbs_expr_free_binary(SbsBinaryExpr *binary_node)
{
    sbs_expr_free(binary_node->left);
    sbs_expr_free(binary_node->right);
    fl_free(binary_node);
}

SbsBinaryExpr* sbs_expr_copy_binary(SbsBinaryExpr *node)
{
    SbsBinaryExpr *copy = fl_malloc(sizeof(SbsBinaryExpr));

    copy->kind = SBS_EXPR_BINARY;
    copy->op = node->op;
    copy->left = sbs_expr_copy(node->left);
    copy->right = sbs_expr_copy(node->right);

    return copy;
}

SbsBinaryExpr* sbs_expr_make_binary(SbsExprOperator op, SbsExpression *left, SbsExpression *right)
{
    if (!fl_array_contains_n(BinaryOperators, flm_array_length(BinaryOperators), &op, sizeof(SbsExprOperator)))
        return NULL;

    SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));

    binode->kind = SBS_EXPR_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return binode;
}


SbsExpression* sbs_expr_parse_comparisson(SbsParser *parser)
{
    SbsExpression *node = sbs_expr_parse_unary(parser);

    while (sbs_parser_has_input(parser))
    {
        bool is_not = false;
        SbsExprOperator operator = SBS_EXPR_OP_UNK;

        if (sbs_parser_next_is(parser, SBS_TOKEN_OP_NOT))
        {
            is_not = true;
            sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        }

        if (sbs_parser_next_is(parser, SBS_TOKEN_OP_IN))
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_IN);
            operator = SBS_EXPR_OP_IN;
        }
        else
        {
            break;
        }

        SbsExpression *right = sbs_expr_parse_unary(parser);

        node = (SbsExpression*) sbs_expr_make_binary(operator, node, right);

        if (is_not)
            node = (SbsExpression*) sbs_expr_make_unary(SBS_EXPR_OP_NOT, node);
    }

    return node;
}

SbsExpression* sbs_expr_parse_equality(SbsParser *parser)
{
    SbsExpression *node = sbs_expr_parse_comparisson(parser);

    while (sbs_parser_has_input(parser))
    {
        SbsExprOperator operator = SBS_EXPR_OP_UNK;
        const SbsToken *operator_token = sbs_parser_peek(parser);

        if (operator_token->type == SBS_TOKEN_OP_EQ)
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_EQ);
            operator = SBS_EXPR_OP_EQ;
        }
        else if (operator_token->type == SBS_TOKEN_OP_NEQ)
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_NEQ);
            operator = SBS_EXPR_OP_NEQ;
        }
        else
        {
            break;
        }

        SbsExpression *right = sbs_expr_parse_comparisson(parser);

        node = (SbsExpression*) sbs_expr_make_binary(operator, node, right);
    }

    return node;
}

SbsExpression* sbs_expr_parse_and(SbsParser *parser)
{
    SbsExpression *node = sbs_expr_parse_equality(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_AND)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_AND);

        SbsExpression *right = sbs_expr_parse_equality(parser);

        node = (SbsExpression*) sbs_expr_make_binary(SBS_EXPR_OP_AND, node, right);
    }

    return node;
}

SbsExpression* sbs_expr_parse_or(SbsParser *parser)
{
    SbsExpression *node = sbs_expr_parse_and(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_OR)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_OR);

        SbsExpression *right = sbs_expr_parse_and(parser);

        node = (SbsExpression*) sbs_expr_make_binary(SBS_EXPR_OP_OR, node, right);
    }

    return node;
}
