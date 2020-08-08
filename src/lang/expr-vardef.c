#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-vardef.h"

void sbs_expr_free_vardef(SbsVarDefinitionExpr *var_def)
{
    if (var_def->name)
        sbs_expr_free((SbsExpression*) var_def->name);

    if (var_def->value)
        sbs_expr_free(var_def->value);

    fl_free(var_def);
}

SbsVarDefinitionExpr* sbs_expr_copy_vardef(SbsVarDefinitionExpr *var_def)
{
    SbsVarDefinitionExpr *copy = fl_malloc(sizeof(SbsVarDefinitionExpr));

    copy->kind = SBS_EXPR_VAR_DEFINITION;
    copy->name = (SbsVariableExpr*) sbs_expr_copy_variable(var_def->name);
    copy->value = sbs_expr_copy(var_def->value);

    return copy;
}

SbsVarDefinitionExpr* sbs_expr_make_vardef(void)
{
    SbsVarDefinitionExpr *var_def = fl_malloc(sizeof(SbsVarDefinitionExpr));
    var_def->kind = SBS_EXPR_VAR_DEFINITION;

    return var_def;
}

SbsVarDefinitionExpr* sbs_expr_parse_vardef(SbsParser *parser)
{
    SbsVarDefinitionExpr *var_def = sbs_expr_make_vardef();

    var_def->name = sbs_expr_parse_variable(parser);
    sbs_parser_consume(parser, SBS_TOKEN_ASSIGN);
    var_def->value = sbs_expr_parse(parser);

    return var_def;
}
