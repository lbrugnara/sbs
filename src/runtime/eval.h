#ifndef SBS_RUNTIME_EVAL_H
#define SBS_RUNTIME_EVAL_H

#include <fllib/containers/Hashtable.h>

typedef enum SbsEvalOperatorKind {
    SBS_EVAL_OP_UNK,
    SBS_EVAL_OP_ID,
    SBS_EVAL_OP_AND,
    SBS_EVAL_OP_OR,
    SBS_EVAL_OP_NOT,
    SBS_EVAL_OP_IN_ARRAY,
} SbsEvalOperatorKind;

typedef enum SbsExprKind {
    SBS_EXPR_UNK,
    SBS_EXPR_UNARY,
    SBS_EXPR_BINARY,
    SBS_EXPR_ARRAY,
    SBS_EXPR_VARIABLE,
    SBS_EXPR_VALUE
} SbsExprKind;

typedef struct SbsExpression {    
    SbsExprKind kind;
} SbsExpression;

typedef struct SbsValueExpr {
    SbsExprKind kind;
    enum {
        SBS_EXPR_VALUE_TYPE_UNK,
        SBS_EXPR_VALUE_TYPE_BOOL,
        SBS_EXPR_VALUE_TYPE_STR,
        SBS_EXPR_VALUE_TYPE_ARRAY,
    } type;
    union {
        bool b;
        char *s;
        SbsExpression **a;
    } value;
} SbsValueExpr;

typedef struct SbsVariableExpr {
    SbsExprKind kind;
    char *name;
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

typedef struct SbsEvalContext {
    FlHashtable *variables;
} SbsEvalContext;

SbsEvalContext* sbs_eval_context_new(void);
void sbs_eval_context_free(SbsEvalContext *context);

bool sbs_expression_eval(SbsEvalContext *context, SbsExpression *node);
void sbs_expression_free(SbsExpression *node);
SbsExpression* sbs_expression_copy(SbsExpression *node);
SbsExpression* sbs_expression_make_binary(SbsEvalOperatorKind op, SbsExpression *left, SbsExpression *right);

#endif /* SBS_RUNTIME_EVAL_H */
