#include <fllib/Cstring.h>
#include "eval.h"
#include "../utils.h"

static SbsValueExpr* eval_run(SbsEvalContext *context, SbsExpression *node);

SbsEvalContext* sbs_eval_context_new(void)
{
    SbsEvalContext *ctx = fl_malloc(sizeof(SbsEvalContext));

    ctx->variables = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_allocator = fl_container_allocator_string,
        .value_cleaner = fl_container_cleaner_pointer
    });

    return ctx;
}

void sbs_eval_context_free(SbsEvalContext *context)
{
    fl_hashtable_free(context->variables);
    fl_free(context);
}

static SbsValueExpr* eval_variable_node(SbsVariableExpr *var_node, SbsEvalContext *context)
{
    SbsValueExpr *result = fl_malloc(sizeof(SbsValueExpr));
    result->kind = SBS_EXPR_VALUE;
    result->type = SBS_EXPR_VALUE_TYPE_UNK;

    if (!fl_hashtable_has_key(context->variables, var_node->name))
        return result;

    char *var_value = fl_hashtable_get(context->variables, var_node->name);

    if (var_value == NULL)
        return result;

    // TODO: By now, we assume STRING type here
    result->type = SBS_EXPR_VALUE_TYPE_STR;
    result->value.s = fl_cstring_dup(var_value);

    return result;
}

static SbsValueExpr* eval_array_node(SbsArrayExpr *array_node, SbsEvalContext *context)
{
    SbsValueExpr *array_val = fl_malloc(sizeof(SbsValueExpr));
    array_val->kind = SBS_EXPR_VALUE;
    array_val->type = SBS_EXPR_VALUE_TYPE_ARRAY;
    array_val->value.a = fl_array_new(sizeof(SbsExpression*), 0);

    for (size_t i=0; i < fl_array_length(array_node->items); i++)
    {
        SbsValueExpr *result = eval_run(context, array_node->items[i]);
        array_val->value.a = fl_array_append(array_val->value.a, &result);
    }

    return array_val;
}

static SbsValueExpr* eval_unary_node(SbsUnaryExpr *unary_node, SbsEvalContext *context)
{
    SbsValueExpr *result = eval_run(context, unary_node->node);

    // TODO: Error handling for non-boolean values or type conversion...
    if (result->type != SBS_EXPR_VALUE_TYPE_BOOL)
        return result;

    if (unary_node->op == SBS_EVAL_OP_NOT)
        result->value.b = !result->value.b;

    return result;
}

static SbsValueExpr* eval_binary_node(SbsBinaryExpr *binary_node, SbsEvalContext *context)
{
    SbsValueExpr *bin_result = fl_malloc(sizeof(SbsValueExpr));
    bin_result->kind = SBS_EXPR_VALUE;
    bin_result->type = SBS_EXPR_VALUE_TYPE_UNK;

    SbsValueExpr *left_result = eval_run(context, binary_node->left);

    switch (binary_node->op)
    {
        case SBS_EVAL_OP_AND:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            // Short circuit
            // TODO: Error handling for non-boolean values or type conversion...
            if (left_result->type != SBS_EXPR_VALUE_TYPE_BOOL || !left_result->value.b)
            {
                sbs_expression_free((SbsExpression*) left_result);
                return bin_result;
            }

            SbsValueExpr *right_result = eval_run(context, binary_node->right);

            // TODO: Error handling for non-boolean values or type conversion...
            if (right_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expression_free((SbsExpression*) left_result);
                sbs_expression_free((SbsExpression*) right_result);
                return bin_result;
            }

            // The result depends on the RHS
            bin_result->value.b = right_result->value.b;
            sbs_expression_free((SbsExpression*) left_result);
            sbs_expression_free((SbsExpression*) right_result);
            return bin_result;            
        }
        case SBS_EVAL_OP_OR:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            // TODO: Error handling for non-boolean values or type conversion...
            if (left_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expression_free((SbsExpression*) left_result);
                return bin_result;
            }

            // Short circuit
            if (left_result->value.b)
            {
                bin_result->value.b = true;
                sbs_expression_free((SbsExpression*) left_result);
                return bin_result;
            }

            SbsValueExpr *right_result = eval_run(context, binary_node->right);

            // TODO: Error handling for non-boolean values or type conversion...
            if (right_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expression_free((SbsExpression*) left_result);
                sbs_expression_free((SbsExpression*) right_result);
                return bin_result;
            }

            // The result depends on the RHS
            bin_result->value.b = right_result->value.b;
            sbs_expression_free((SbsExpression*) left_result);
            sbs_expression_free((SbsExpression*) right_result);
            return bin_result;            
        }
        case SBS_EVAL_OP_IN_ARRAY:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = eval_run(context, binary_node->right);

            if (right_result->type != SBS_EXPR_VALUE_TYPE_ARRAY)
            {
                sbs_expression_free((SbsExpression*) left_result);
                sbs_expression_free((SbsExpression*) right_result);
                return bin_result;
            }

            bool in_array = false;
            for (size_t i=0; !in_array && i < fl_array_length(right_result->value.a); i++)
            {
                SbsExpression *node = right_result->value.a[i];

                if (node->kind != SBS_EXPR_VALUE)
                    break;

                SbsValueExpr *item = (SbsValueExpr*) node;

                // We don't check for arrays within arrays (by now at least..)
                if (item->type == SBS_EXPR_VALUE_TYPE_UNK || item->type == SBS_EXPR_VALUE_TYPE_ARRAY || item->type != left_result->type)
                    break;

                switch (item->type)
                {
                    case SBS_EXPR_VALUE_TYPE_BOOL:
                        in_array = item->value.b != left_result->value.b;
                        break;

                    case SBS_EXPR_VALUE_TYPE_STR:
                        in_array = flm_cstring_equals(item->value.s, left_result->value.s);
                        break;

                    default: break;
                }
            }

            sbs_expression_free((SbsExpression*) left_result);
            sbs_expression_free((SbsExpression*) right_result);

            bin_result->value.b = in_array;

            return bin_result;
        }

        default: break;
    }

    return false;
}

static SbsValueExpr* eval_run(SbsEvalContext *context, SbsExpression *node)
{
    SbsValueExpr *result = NULL;

    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            result = (SbsValueExpr*) sbs_expression_copy(node);
            break;

        case SBS_EXPR_VARIABLE:
            result = eval_variable_node((SbsVariableExpr*) node, context);
            break;

        case SBS_EXPR_ARRAY:
            result = eval_array_node((SbsArrayExpr*) node, context);
            break;

        case SBS_EXPR_UNARY:
            result = eval_unary_node((SbsUnaryExpr*) node, context);
            break;

        case SBS_EXPR_BINARY:
            result = eval_binary_node((SbsBinaryExpr*) node, context);
            break;

        default: break;
    }

    return result;
}

bool sbs_expression_eval(SbsEvalContext *context, SbsExpression *node)
{
    SbsExpression *node_copy = sbs_expression_copy(node);

    SbsValueExpr *result = eval_run(context, node_copy);

    bool b = false;
    
    if (result != NULL && result->type == SBS_EXPR_VALUE_TYPE_BOOL)
        b = result->value.b;

    sbs_expression_free((SbsExpression*) result);
    sbs_expression_free(node_copy);

    return b;
}

static void free_value_node(SbsValueExpr *value)
{
    if (value->type == SBS_EXPR_VALUE_TYPE_STR)
    {
        fl_cstring_free(value->value.s);
    }
    else if (value->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        fl_array_free_each_pointer(value->value.a, (FlArrayFreeElementFunc) sbs_expression_free);
    }

    fl_free(value);
}

static void free_variable_node(SbsVariableExpr *value_node)
{
    if (value_node->name)
        fl_cstring_free(value_node->name);

    fl_free(value_node);
}

static void free_array_node(SbsArrayExpr *value_node)
{
    if (value_node->items)
        fl_array_free_each_pointer(value_node->items, (FlArrayFreeElementFunc) sbs_expression_free);

    fl_free(value_node);
}

static void free_unary_node(SbsUnaryExpr *unary_node)
{
    sbs_expression_free(unary_node->node);
    fl_free(unary_node);
}

static void free_binary_node(SbsBinaryExpr *binary_node)
{
    sbs_expression_free(binary_node->left);
    sbs_expression_free(binary_node->right);
    fl_free(binary_node);
}

void sbs_expression_free(SbsExpression *node)
{
    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            free_value_node((SbsValueExpr*) node);
            break;

        case SBS_EXPR_VARIABLE:
            free_variable_node((SbsVariableExpr*) node);
            break;

        case SBS_EXPR_ARRAY:
            free_array_node((SbsArrayExpr*) node);
            break;

        case SBS_EXPR_UNARY:
            free_unary_node((SbsUnaryExpr*) node);
            break;

        case SBS_EXPR_BINARY:
            free_binary_node((SbsBinaryExpr*) node);
            break;

        default:
            return;
    }
}

static SbsExpression* copy_value_node(SbsValueExpr* node)
{
    SbsValueExpr *copy = fl_malloc(sizeof(SbsValueExpr));

    copy->kind = node->kind;
    copy->type = node->type;

    if (node->type == SBS_EXPR_VALUE_TYPE_BOOL)
    {
        copy->value.b = node->value.b;
    }
    else if (node->type == SBS_EXPR_VALUE_TYPE_STR)
    {
        copy->value.s = fl_cstring_dup(node->value.s);
    }
    else if (node->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        copy->value.a = fl_array_new(sizeof(SbsExpression*), fl_array_length(node->value.a));

        for (size_t i=0; i < fl_array_length(node->value.a); i++)
        {
            SbsExpression *item_copy = sbs_expression_copy(node->value.a[i]);
            copy->value.a = fl_array_append(copy->value.a, &item_copy);
        }
    }

    return (SbsExpression*) copy;
}

static SbsExpression* copy_variable_node(SbsVariableExpr* node)
{
    SbsVariableExpr *copy = fl_malloc(sizeof(SbsVariableExpr));

    copy->kind = node->kind;
    copy->name = fl_cstring_dup(node->name);

    return (SbsExpression*) copy;
}

static SbsExpression* copy_array_node(SbsArrayExpr* node)
{
    SbsArrayExpr *copy = fl_malloc(sizeof(SbsArrayExpr));

    copy->kind = node->kind;
    copy->items = fl_array_new(sizeof(SbsExpression*), fl_array_length(node->items));

    for (size_t i=0; i < fl_array_length(node->items); i++)
    {
        copy->items[i] = sbs_expression_copy(node->items[i]);
    }

    return (SbsExpression*) copy;
}

static SbsExpression* copy_unary_node(SbsUnaryExpr* node)
{
    SbsUnaryExpr *copy = fl_malloc(sizeof(SbsUnaryExpr));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->node = sbs_expression_copy(node->node);

    return (SbsExpression*) copy;
}

static SbsExpression* copy_binary_node(SbsBinaryExpr* node)
{
    SbsBinaryExpr *copy = fl_malloc(sizeof(SbsBinaryExpr));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->left = sbs_expression_copy(node->left);
    copy->right = sbs_expression_copy(node->right);

    return (SbsExpression*) copy;
}

SbsExpression* sbs_expression_copy(SbsExpression *node)
{
    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            return copy_value_node((SbsValueExpr*) node);

        case SBS_EXPR_VARIABLE:
            return copy_variable_node((SbsVariableExpr*) node);

        case SBS_EXPR_ARRAY:
            return copy_array_node((SbsArrayExpr*) node);

        case SBS_EXPR_UNARY:
            return copy_unary_node((SbsUnaryExpr*) node);

        case SBS_EXPR_BINARY:
            return copy_binary_node((SbsBinaryExpr*) node);

        default:
            break;
    }

    return NULL;
}

SbsExpression* sbs_expression_make_binary(SbsEvalOperatorKind op, SbsExpression *left, SbsExpression *right)
{
    SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));

    binode->kind = SBS_EXPR_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return (SbsExpression*) binode;
}