#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-value.h"

void sbs_expr_free_value(SbsValueExpr *value)
{
    if (value->type == SBS_EXPR_VALUE_TYPE_STRING)
    {
        fl_cstring_free(value->value.s);
    }
    else if (value->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        fl_array_free_each_pointer(value->value.a, (FlArrayFreeElementFunc) sbs_expr_free);
    }

    fl_free(value);
}

SbsValueExpr* sbs_expr_copy_value(SbsValueExpr* node)
{
    SbsValueExpr *copy = fl_malloc(sizeof(SbsValueExpr));

    copy->kind = SBS_EXPR_VALUE;
    copy->type = node->type;

    if (node->type == SBS_EXPR_VALUE_TYPE_BOOL)
    {
        copy->value.b = node->value.b;
    }
    else if (node->type == SBS_EXPR_VALUE_TYPE_STRING)
    {
        copy->value.s = fl_cstring_dup(node->value.s);
    }
    else if (node->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        copy->value.a = fl_array_new(sizeof(SbsExpression*), fl_array_length(node->value.a));

        for (size_t i=0; i < fl_array_length(node->value.a); i++)
        {
            SbsExpression *item_copy = sbs_expr_copy(node->value.a[i]);
            copy->value.a = fl_array_append(copy->value.a, &item_copy);
        }
    }

    return copy;
}

SbsValueExpr* sbs_expr_make_value(SbsValueExprType type)
{
    SbsValueExpr *value = fl_malloc(sizeof(SbsValueExpr));

    value->kind = SBS_EXPR_VALUE;
    value->type = type;

    return value;
}
