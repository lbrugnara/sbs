#ifndef SBS_LANG_EXPR_UNARY_H
#define SBS_LANG_EXPR_UNARY_H

#include "expr.h"

typedef struct SbsUnaryExpr {
    SbsExpressionKind kind;
    SbsExprOperator op;
    SbsExpression *node;
} SbsUnaryExpr;

SbsExpression* sbs_expr_parse_primary(SbsParser *parser);
SbsExpression* sbs_expr_parse_unary(SbsParser *parser);
SbsUnaryExpr* sbs_expr_make_unary(SbsExprOperator op, SbsExpression *left);
void sbs_expr_free_unary(SbsUnaryExpr *unary_node);
SbsUnaryExpr* sbs_expr_copy_unary(SbsUnaryExpr* node);

#endif /* SBS_LANG_EXPR_UNARY_H */
