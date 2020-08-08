#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-array.h"

void sbs_expr_free_array(SbsArrayExpr *value_node)
{
    if (value_node->items)
        fl_array_free_each_pointer(value_node->items, (FlArrayFreeElementFunc) sbs_expr_free);

    fl_free(value_node);
}

SbsArrayExpr* sbs_expr_copy_array(SbsArrayExpr* node)
{
    SbsArrayExpr *copy = fl_malloc(sizeof(SbsArrayExpr));

    copy->kind = SBS_EXPR_ARRAY;
    copy->items = fl_array_new(sizeof(SbsExpression*), fl_array_length(node->items));

    for (size_t i=0; i < fl_array_length(node->items); i++)
    {
        copy->items[i] = sbs_expr_copy(node->items[i]);
    }

    return copy;
}

SbsArrayExpr* sbs_expr_make_array(void)
{
    SbsArrayExpr *array_node = fl_malloc(sizeof(SbsArrayExpr));

    array_node->kind = SBS_EXPR_ARRAY;
    array_node->items = fl_array_new(sizeof(SbsExpression*), 0);

    return array_node;
}

void sbs_expr_array_add_item(SbsArrayExpr *array, SbsExpression *item)
{
    array->items = fl_array_append(array->items, &item);
}

SbsArrayExpr* sbs_expr_parse_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    SbsArrayExpr *array_node = sbs_expr_make_array();

    while (sbs_parser_next_is_not(parser, SBS_TOKEN_RBRACKET))
    {
        SbsExpression *item = sbs_expr_parse(parser);

        if (item != NULL)
        {
            sbs_expr_array_add_item(array_node, (SbsExpression*) item);
        }

        if (sbs_parser_next_is_not(parser, SBS_TOKEN_RBRACKET))
            sbs_parser_consume(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return array_node;
}
