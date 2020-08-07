#ifndef SBS_LANG_EXPRESSION_H
#define SBS_LANG_EXPRESSION_H

#include "variable.h"
#include "string.h"

typedef enum SbsEvalOperatorKind {
    SBS_EVAL_OP_UNK,
    SBS_EVAL_OP_ID,
    SBS_EVAL_OP_AND,
    SBS_EVAL_OP_OR,
    SBS_EVAL_OP_NOT,
    SBS_EVAL_OP_EQ,
    SBS_EVAL_OP_NEQ,
    SBS_EVAL_OP_IN,
} SbsEvalOperatorKind;

typedef enum SbsExprKind {
    SBS_EXPR_UNK,
    SBS_EXPR_UNARY,
    SBS_EXPR_BINARY,
    SBS_EXPR_ARRAY,
    SBS_EXPR_VARIABLE,
    SBS_EXPR_VALUE,
    SBS_EXPR_STRING,
    SBS_EXPR_IF,
} SbsExprKind;

typedef struct SbsExpression {    
    SbsExprKind kind;
} SbsExpression;

typedef enum SbsValueExprType {
    SBS_EXPR_VALUE_TYPE_UNK,
    SBS_EXPR_VALUE_TYPE_BOOL,
    SBS_EXPR_VALUE_TYPE_STRING,
    SBS_EXPR_VALUE_TYPE_ARRAY,
} SbsValueExprType;

typedef struct SbsValueExpr {
    SbsExprKind kind;
    SbsValueExprType type;
    union {
        bool b;
        char *s;
        SbsExpression **a;
    } value;
} SbsValueExpr;

typedef struct SbsStringExpr {
    SbsExprKind kind;
    struct SbsString *value;
} SbsStringExpr;

typedef struct SbsVariableExpr {
    SbsExprKind kind;
    SbsVariable *info;
} SbsVariableExpr;

typedef struct SbsArrayExpr {
    SbsExprKind kind;
    SbsExpression **items;
} SbsArrayExpr;

typedef struct SbsUnaryExpr {
    SbsExprKind kind;
    SbsEvalOperatorKind op;
    SbsExpression *node;
} SbsUnaryExpr;

typedef struct SbsBinaryExpr {
    SbsExprKind kind;
    SbsEvalOperatorKind op;
    SbsExpression *left;
    SbsExpression *right;
} SbsBinaryExpr;

typedef struct SbsIfExpr {
    SbsExprKind kind;
    SbsExpression *condition;
    SbsExpression *then_branch;
    SbsExpression *else_branch;
} SbsIfExpr;

SbsValueExpr* sbs_expression_make_value(SbsValueExprType type);
SbsArrayExpr* sbs_expression_make_array(void);
void sbs_expression_array_add_item(SbsArrayExpr *array, SbsExpression *item);
SbsVariableExpr* sbs_expression_make_variable(const char *name, const char *namespace);
SbsUnaryExpr* sbs_expression_make_unary(SbsEvalOperatorKind op, SbsExpression *left);
SbsBinaryExpr* sbs_expression_make_binary(SbsEvalOperatorKind op, SbsExpression *left, SbsExpression *right);
SbsIfExpr* sbs_expression_make_if(SbsExpression *condition, SbsExpression *then_branch, SbsExpression *else_branch);
SbsStringExpr* sbs_expression_make_string(struct SbsString *string);
void sbs_expression_free(SbsExpression *node);
SbsExpression* sbs_expression_copy(SbsExpression *node);

#endif /* SBS_LANG_EXPRESSION_H */
