#ifndef SBS_LANG_EXPR_ARRAY_H
#define SBS_LANG_EXPR_ARRAY_H

#include "expr.h"

typedef struct SbsArrayExpr {
    SbsExpressionKind kind;
    SbsExpression **items;
} SbsArrayExpr;

SbsArrayExpr* sbs_expr_parse_array(SbsParser *parser);
SbsArrayExpr* sbs_expr_make_array(void);
void sbs_expr_array_add_item(SbsArrayExpr *array, SbsExpression *item);
void sbs_expr_free_array(SbsArrayExpr *value_node);
SbsArrayExpr* sbs_expr_copy_array(SbsArrayExpr* node);

#endif /* SBS_LANG_EXPR_ARRAY_H */
