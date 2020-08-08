#ifndef SBS_LANG_EXPR_H
#define SBS_LANG_EXPR_H

#include "parser.h"
#include "../utils.h"

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
    SBS_EXPR_VAR_DEFINITION,
    SBS_EXPR_IF,
} SbsExpressionKind;

typedef struct SbsExpression {    
    SbsExpressionKind kind;
} SbsExpression;

SbsExpression* sbs_expr_parse(SbsParser *parser);
SbsExpression* sbs_expr_parse_for(SbsParser *parser);
void sbs_expr_free(SbsExpression *node);
SbsExpression* sbs_expr_copy(SbsExpression *node);

#endif /* SBS_LANG_EXPR_H */
