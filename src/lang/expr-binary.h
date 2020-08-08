#ifndef SBS_LANG_EXPR_BINARY_H
#define SBS_LANG_EXPR_BINARY_H

#include "expr.h"

typedef struct SbsBinaryExpr {
    SbsExpressionKind kind;
    SbsExprOperator op;
    SbsExpression *left;
    SbsExpression *right;
} SbsBinaryExpr;

SbsExpression* sbs_expr_parse_comparisson(SbsParser *parser);
SbsExpression* sbs_expr_parse_equality(SbsParser *parser);
SbsExpression* sbs_expr_parse_and(SbsParser *parser);
SbsExpression* sbs_expr_parse_or(SbsParser *parser);
SbsBinaryExpr* sbs_expr_make_binary(SbsExprOperator op, SbsExpression *left, SbsExpression *right);
void sbs_expr_free_binary(SbsBinaryExpr *binary_node);
SbsBinaryExpr* sbs_expr_copy_binary(SbsBinaryExpr *node);

#endif /* SBS_LANG_EXPR_BINARY_H */
