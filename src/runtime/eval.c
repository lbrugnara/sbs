#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "eval.h"
#include "../utils.h"

static SbsEvalOperatorKind UnaryOperators[] = {
    SBS_EVAL_OP_ID,
    SBS_EVAL_OP_NOT,
};

static SbsEvalOperatorKind BinaryOperators[] = {
    SBS_EVAL_OP_OR,
    SBS_EVAL_OP_AND,
    SBS_EVAL_OP_EQ,
    SBS_EVAL_OP_NEQ,
    SBS_EVAL_OP_IN,
};

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

    if (flm_cstring_equals(var_node->info->namespace, "env"))
    {
        #ifdef _WIN32
        {
            size_t req_size = 0;
            getenv_s( &req_size, NULL, 0, var_node->info->name);

            if (req_size == 0)
                return result;

            char *var_value = fl_cstring_new(req_size);
            if (getenv_s(&req_size, var_value, req_size, var_node->info->name) != 0)
            {
                fl_cstring_free(var_value);
                return result;
            }

            result->type = SBS_EXPR_VALUE_TYPE_STR;
            result->value.s = var_value;

            return result;
        }
        #else
        {
            char *var_value = getenv(var_node->info->name);

            if (var_value == NULL)
                return result;

            result->type = SBS_EXPR_VALUE_TYPE_STR;
            result->value.s = fl_cstring_dup(var_value);

            return result;
        }
        #endif        
    }
    else
    {
        if (!fl_hashtable_has_key(context->variables, var_node->info->fqn))
            return result;

        char *var_value = fl_hashtable_get(context->variables, var_node->info->fqn);

        if (var_value == NULL)
            return result;

        // TODO: By now, we assume STRING type here
        result->type = SBS_EXPR_VALUE_TYPE_STR;
        result->value.s = fl_cstring_dup(var_value);
    }

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
        case SBS_EVAL_OP_EQ:
        {
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = eval_run(context, binary_node->right);

            if (left_result->type != right_result->type)
            {
                sbs_expression_free((SbsExpression*) left_result);
                sbs_expression_free((SbsExpression*) right_result);
                return bin_result;
            }

            switch (left_result->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    bin_result->value.b = left_result->value.b == right_result->value.b;
                    break;

                case SBS_EXPR_VALUE_TYPE_STR:
                    bin_result->value.b = flm_cstring_equals(left_result->value.s, right_result->value.s);
                    break;

                default: break;
            }

            sbs_expression_free((SbsExpression*) left_result);
            sbs_expression_free((SbsExpression*) right_result);
            return bin_result;
        }
        case SBS_EVAL_OP_NEQ:
        {
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = eval_run(context, binary_node->right);

            if (left_result->type != right_result->type)
            {
                bin_result->value.b = true;
                sbs_expression_free((SbsExpression*) left_result);
                sbs_expression_free((SbsExpression*) right_result);
                return bin_result;
            }

            switch (left_result->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    bin_result->value.b = left_result->value.b != right_result->value.b;
                    break;

                case SBS_EXPR_VALUE_TYPE_STR:
                    bin_result->value.b = !flm_cstring_equals(left_result->value.s, right_result->value.s);
                    break;

                default: break;
            }

            sbs_expression_free((SbsExpression*) left_result);
            sbs_expression_free((SbsExpression*) right_result);
            return bin_result;
        }
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
        case SBS_EVAL_OP_IN:
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
    if (value_node->info)
        sbs_varinfo_free(value_node->info);

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
    copy->info = sbs_varinfo_new(node->info->name, node->info->namespace);

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

SbsValueExpr* sbs_expression_make_value(SbsValueExprType type)
{
    SbsValueExpr *value = fl_malloc(sizeof(SbsValueExpr));

    value->kind = SBS_EXPR_VALUE;
    value->type = type;

    return value;
}

SbsArrayExpr* sbs_expression_make_array(void)
{
    SbsArrayExpr *array_node = fl_malloc(sizeof(SbsArrayExpr));

    array_node->kind = SBS_EXPR_ARRAY;
    array_node->items = fl_array_new(sizeof(SbsExpression*), 0);

    return array_node;
}

void sbs_expression_array_add_item(SbsArrayExpr *array, SbsExpression *item)
{
    array->items = fl_array_append(array->items, &item);
}

SbsVariableExpr* sbs_expression_make_variable(const char *name, const char *namespace)
{
    SbsVariableExpr *var_node = fl_malloc(sizeof(SbsVariableExpr));

    var_node->kind = SBS_EXPR_VARIABLE;
    var_node->info = sbs_varinfo_new(name, namespace);

    return var_node;
}

SbsUnaryExpr* sbs_expression_make_unary(SbsEvalOperatorKind op, SbsExpression *left)
{
    if (!fl_array_contains_n(UnaryOperators, flm_array_length(UnaryOperators), &op, sizeof(SbsEvalOperatorKind)))
        return NULL;

    SbsUnaryExpr *unary_node = fl_malloc(sizeof(SbsUnaryExpr));

    unary_node->kind = SBS_EXPR_UNARY;
    unary_node->op = op;
    unary_node->node = left;

    return unary_node;
}

SbsBinaryExpr* sbs_expression_make_binary(SbsEvalOperatorKind op, SbsExpression *left, SbsExpression *right)
{
    if (!fl_array_contains_n(BinaryOperators, flm_array_length(BinaryOperators), &op, sizeof(SbsEvalOperatorKind)))
        return NULL;

    SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));

    binode->kind = SBS_EXPR_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return binode;
}
