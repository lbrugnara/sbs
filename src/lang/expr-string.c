#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-string.h"

void sbs_expr_free_string(SbsStringExpr *str_expr)
{
    sbs_string_free(str_expr->value);
    fl_free(str_expr);
}

SbsStringExpr* sbs_expr_copy_string(SbsStringExpr *string)
{
    SbsStringExpr *copy = fl_malloc(sizeof(SbsStringExpr));

    copy->kind = SBS_EXPR_STRING;
    copy->value = sbs_string_copy(string->value);

    return copy;
}

SbsStringExpr* sbs_expr_make_string(SbsString *string)
{
    SbsStringExpr *str_expr = fl_malloc(sizeof(SbsStringExpr));

    str_expr->kind = SBS_EXPR_STRING;
    str_expr->value = string;

    return str_expr;
}

SbsStringExpr* sbs_expr_parse_string(SbsParser *parser)
{
    return sbs_expr_make_string(sbs_string_parse(parser));
}
