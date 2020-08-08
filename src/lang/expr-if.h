#ifndef SBS_LANG_EXPR_IF_H
#define SBS_LANG_EXPR_IF_H

#include "expr.h"

typedef struct SbsIfExpr {
    SbsExpressionKind kind;
    SbsExpression *condition;
    SbsExpression *then_branch;
    SbsExpression *else_branch;
} SbsIfExpr;

SbsExpression* sbs_expr_parse_conditional(SbsParser *parser);
SbsExpression* sbs_expr_parse_if(SbsParser *parser);
SbsIfExpr* sbs_expr_make_if(SbsExpression *condition, SbsExpression *then_branch, SbsExpression *else_branch);
void sbs_expr_free_if(SbsIfExpr *if_node);
SbsIfExpr* sbs_expr_copy_if(SbsIfExpr *if_expr);

#endif /* SBS_LANG_EXPR_IF_H */
