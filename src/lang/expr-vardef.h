#ifndef SBS_LANG_EXPR_VARDEF_H
#define SBS_LANG_EXPR_VARDEF_H

#include "expr.h"
#include "expr-variable.h"

typedef struct SbsVarDefinitionExpr {
    SbsExpressionKind kind;
    SbsVariableExpr *name;
    SbsExpression *value;
} SbsVarDefinitionExpr;

SbsVarDefinitionExpr* sbs_expr_parse_vardef(SbsParser *parser);
SbsVarDefinitionExpr* sbs_expr_make_vardef(void);
void sbs_expr_free_vardef(SbsVarDefinitionExpr *var_def);
SbsVarDefinitionExpr* sbs_expr_copy_vardef(SbsVarDefinitionExpr *var_def);

#endif /* SBS_LANG_EXPR_VARDEF_H */
