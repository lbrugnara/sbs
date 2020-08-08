#ifndef SBS_LANG_EXPR_VARIABLE_H
#define SBS_LANG_EXPR_VARIABLE_H

#include "expr.h"

typedef struct SbsVariableExpr {
    SbsExpressionKind kind;
    const char *namespace;
    const char *name;
    const char *fqn;
} SbsVariableExpr;

SbsVariableExpr* sbs_expr_parse_variable(SbsParser *parser);
SbsVariableExpr* sbs_expr_make_variable(const char *name, const char *namespace);
void sbs_expr_free_variable(SbsVariableExpr *var_node);
SbsVariableExpr* sbs_expr_copy_variable(SbsVariableExpr* node);

#endif /* SBS_LANG_EXPR_VARIABLE_H */
