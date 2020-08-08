#ifndef SBS_LANG_EXPR_VALUE_H
#define SBS_LANG_EXPR_VALUE_H

#include "expr.h"

typedef enum SbsValueExprType {
    SBS_EXPR_VALUE_TYPE_UNK,
    SBS_EXPR_VALUE_TYPE_BOOL,
    SBS_EXPR_VALUE_TYPE_STRING,
    SBS_EXPR_VALUE_TYPE_ARRAY,
} SbsValueExprType;

typedef struct SbsValueExpr {
    SbsExpressionKind kind;
    SbsValueExprType type;
    union {
        bool b;
        char *s;
        SbsExpression **a;
    } value;
} SbsValueExpr;

SbsValueExpr* sbs_expr_make_value(SbsValueExprType type);
void sbs_expr_free_value(SbsValueExpr *value);
SbsValueExpr* sbs_expr_copy_value(SbsValueExpr* node);

#endif /* SBS_LANG_EXPR_VALUE_H */
