#ifndef SBS_LANG_EXPRESSION_H
#define SBS_LANG_EXPRESSION_H

#include "variable.h"
#include "string.h"

typedef enum SbsExprOperator {
    SBS_EXPR_OP_UNK,
    SBS_EXPR_OP_ID,
    SBS_EXPR_OP_AND,
    SBS_EXPR_OP_OR,
    SBS_EXPR_OP_NOT,
    SBS_EXPR_OP_EQ,
    SBS_EXPR_OP_NEQ,
    SBS_EXPR_OP_IN,
} SbsExprOperator;

typedef enum SbsExpressionKind {
    SBS_EXPR_UNK,
    SBS_EXPR_UNARY,
    SBS_EXPR_BINARY,
    SBS_EXPR_ARRAY,
    SBS_EXPR_VARIABLE,
    SBS_EXPR_VALUE,
    SBS_EXPR_STRING,
    SBS_EXPR_IDENTIFIER,
    SBS_EXPR_IF,
} SbsExpressionKind;

typedef struct SbsExpression {    
    SbsExpressionKind kind;
} SbsExpression;

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

typedef struct SbsStringExpr {
    SbsExpressionKind kind;
    struct SbsString *value;
} SbsStringExpr;

typedef struct SbsIdentifierExpr {
    SbsExpressionKind kind;
    char *name;
} SbsIdentifierExpr;

typedef struct SbsVariableExpr {
    SbsExpressionKind kind;
    SbsVariable *info;
} SbsVariableExpr;

typedef struct SbsArrayExpr {
    SbsExpressionKind kind;
    SbsExpression **items;
} SbsArrayExpr;

typedef struct SbsUnaryExpr {
    SbsExpressionKind kind;
    SbsExprOperator op;
    SbsExpression *node;
} SbsUnaryExpr;

typedef struct SbsBinaryExpr {
    SbsExpressionKind kind;
    SbsExprOperator op;
    SbsExpression *left;
    SbsExpression *right;
} SbsBinaryExpr;

typedef struct SbsIfExpr {
    SbsExpressionKind kind;
    SbsExpression *condition;
    SbsExpression *then_branch;
    SbsExpression *else_branch;
} SbsIfExpr;

SbsValueExpr* sbs_expression_make_value(SbsValueExprType type);
SbsArrayExpr* sbs_expression_make_array(void);
void sbs_expression_array_add_item(SbsArrayExpr *array, SbsExpression *item);
SbsVariableExpr* sbs_expression_make_variable(const char *name, const char *namespace);
SbsUnaryExpr* sbs_expression_make_unary(SbsExprOperator op, SbsExpression *left);
SbsBinaryExpr* sbs_expression_make_binary(SbsExprOperator op, SbsExpression *left, SbsExpression *right);
SbsIfExpr* sbs_expression_make_if(SbsExpression *condition, SbsExpression *then_branch, SbsExpression *else_branch);
SbsStringExpr* sbs_expression_make_string(struct SbsString *string);
SbsIdentifierExpr* sbs_expression_make_identifier(char *id);
void sbs_expression_free(SbsExpression *node);
SbsExpression* sbs_expression_copy(SbsExpression *node);

static inline SbsStringExpr** sbs_expression_string_array_extend(SbsStringExpr **dest, SbsStringExpr **source)
{    
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_expression_copy);
}

#endif /* SBS_LANG_EXPRESSION_H */
