#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "eval.h"
#include "../lang/expr-array.h"
#include "../lang/expr-binary.h"
#include "../lang/expr-identifier.h"
#include "../lang/expr-if.h"
#include "../lang/expr-string.h"
#include "../lang/expr-unary.h"
#include "../lang/expr-value.h"
#include "../lang/expr-vardef.h"
#include "../lang/expr-variable.h"

static SbsValueExpr* internal_eval(SbsEvalContext *context, SbsExpression *node);

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

char* sbs_eval_context_add_variable(SbsEvalContext *context, const char *name, const char *value)
{
    return (char*) fl_hashtable_add(context->variables, name, value);
}

char* sbs_eval_context_get_variable(SbsEvalContext *context, const char *name)
{
    return (char*) fl_hashtable_get(context->variables, name);
}

void sbs_eval_context_remove_variable(SbsEvalContext *context, const char *name)
{
    fl_hashtable_remove(context->variables, name, true, true);
}

void sbs_eval_context_merge(SbsEvalContext *dest_context, SbsEvalContext *src_context)
{
    char **variables = fl_hashtable_keys(src_context->variables);

    for (size_t i=0; i < fl_array_length(variables); i++)
        sbs_eval_context_add_variable(dest_context, variables[i], sbs_eval_context_get_variable(src_context, variables[i]));

    fl_array_free(variables);
}

void sbs_eval_context_free(SbsEvalContext *context)
{
    fl_hashtable_free(context->variables);
    fl_free(context);
}

static SbsValueExpr* eval_value_node(SbsValueExpr *node, SbsEvalContext *context)
{
    return (SbsValueExpr*) sbs_expr_copy((SbsExpression*) node);
}

static SbsValueExpr* eval_variable_node(SbsVariableExpr *var_node, SbsEvalContext *context)
{
    SbsValueExpr *result = fl_malloc(sizeof(SbsValueExpr));
    result->kind = SBS_EXPR_VALUE;
    result->type = SBS_EXPR_VALUE_TYPE_UNK;

    if (var_node->namespace && flm_cstring_equals(var_node->namespace, "env"))
    {
        #ifdef _WIN32
        {
            size_t req_size = 0;
            getenv_s( &req_size, NULL, 0, var_node->name);

            if (req_size == 0)
                return result;

            char *var_value = fl_cstring_new(req_size);
            if (getenv_s(&req_size, var_value, req_size, var_node->name) != 0)
            {
                fl_cstring_free(var_value);
                return result;
            }

            result->type = SBS_EXPR_VALUE_TYPE_STRING;
            result->value.s = var_value;

            return result;
        }
        #else
        {
            char *var_value = getenv(var_node->name);

            if (var_value == NULL)
                return result;

            result->type = SBS_EXPR_VALUE_TYPE_STRING;
            result->value.s = fl_cstring_dup(var_value);

            return result;
        }
        #endif        
    }
    else
    {
        if (!fl_hashtable_has_key(context->variables, var_node->fqn))
            return result;

        char *var_value = fl_hashtable_get(context->variables, var_node->fqn);

        if (var_value == NULL)
            return result;

        // TODO: By now, we assume STRING type here
        result->type = SBS_EXPR_VALUE_TYPE_STRING;
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
        SbsValueExpr *result = internal_eval(context, array_node->items[i]);
        array_val->value.a = fl_array_append(array_val->value.a, &result);
    }

    return array_val;
}

static SbsValueExpr* eval_unary_node(SbsUnaryExpr *unary_node, SbsEvalContext *context)
{
    SbsValueExpr *result = internal_eval(context, unary_node->node);

    // TODO: Error handling for non-boolean values or type conversion...
    if (result->type != SBS_EXPR_VALUE_TYPE_BOOL)
        return result;

    if (unary_node->op == SBS_EXPR_OP_NOT)
        result->value.b = !result->value.b;

    return result;
}

static SbsValueExpr* eval_binary_node(SbsBinaryExpr *binary_node, SbsEvalContext *context)
{
    SbsValueExpr *bin_result = fl_malloc(sizeof(SbsValueExpr));
    bin_result->kind = SBS_EXPR_VALUE;
    bin_result->type = SBS_EXPR_VALUE_TYPE_UNK;

    SbsValueExpr *left_result = internal_eval(context, binary_node->left);

    switch (binary_node->op)
    {
        case SBS_EXPR_OP_EQ:
        {
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = internal_eval(context, binary_node->right);

            if (left_result->type != right_result->type)
            {
                sbs_expr_free((SbsExpression*) left_result);
                sbs_expr_free((SbsExpression*) right_result);
                return bin_result;
            }

            switch (left_result->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    bin_result->value.b = left_result->value.b == right_result->value.b;
                    break;

                case SBS_EXPR_VALUE_TYPE_STRING:
                    bin_result->value.b = flm_cstring_equals(left_result->value.s, right_result->value.s);
                    break;

                default: break;
            }

            sbs_expr_free((SbsExpression*) left_result);
            sbs_expr_free((SbsExpression*) right_result);
            return bin_result;
        }
        case SBS_EXPR_OP_NEQ:
        {
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = internal_eval(context, binary_node->right);

            if (left_result->type != right_result->type)
            {
                bin_result->value.b = true;
                sbs_expr_free((SbsExpression*) left_result);
                sbs_expr_free((SbsExpression*) right_result);
                return bin_result;
            }

            switch (left_result->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    bin_result->value.b = left_result->value.b != right_result->value.b;
                    break;

                case SBS_EXPR_VALUE_TYPE_STRING:
                    bin_result->value.b = !flm_cstring_equals(left_result->value.s, right_result->value.s);
                    break;

                default: break;
            }

            sbs_expr_free((SbsExpression*) left_result);
            sbs_expr_free((SbsExpression*) right_result);
            return bin_result;
        }
        case SBS_EXPR_OP_AND:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            // Short circuit
            // TODO: Error handling for non-boolean values or type conversion...
            if (left_result->type != SBS_EXPR_VALUE_TYPE_BOOL || !left_result->value.b)
            {
                sbs_expr_free((SbsExpression*) left_result);
                return bin_result;
            }

            SbsValueExpr *right_result = internal_eval(context, binary_node->right);

            // TODO: Error handling for non-boolean values or type conversion...
            if (right_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expr_free((SbsExpression*) left_result);
                sbs_expr_free((SbsExpression*) right_result);
                return bin_result;
            }

            // The result depends on the RHS
            bin_result->value.b = right_result->value.b;
            sbs_expr_free((SbsExpression*) left_result);
            sbs_expr_free((SbsExpression*) right_result);
            return bin_result;            
        }
        case SBS_EXPR_OP_OR:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            // TODO: Error handling for non-boolean values or type conversion...
            if (left_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expr_free((SbsExpression*) left_result);
                return bin_result;
            }

            // Short circuit
            if (left_result->value.b)
            {
                bin_result->value.b = true;
                sbs_expr_free((SbsExpression*) left_result);
                return bin_result;
            }

            SbsValueExpr *right_result = internal_eval(context, binary_node->right);

            // TODO: Error handling for non-boolean values or type conversion...
            if (right_result->type != SBS_EXPR_VALUE_TYPE_BOOL)
            {
                sbs_expr_free((SbsExpression*) left_result);
                sbs_expr_free((SbsExpression*) right_result);
                return bin_result;
            }

            // The result depends on the RHS
            bin_result->value.b = right_result->value.b;
            sbs_expr_free((SbsExpression*) left_result);
            sbs_expr_free((SbsExpression*) right_result);
            return bin_result;            
        }
        case SBS_EXPR_OP_IN:
        {
            // At this point we know it needs to be bool
            bin_result->type = SBS_EXPR_VALUE_TYPE_BOOL;
            bin_result->value.b = false;

            SbsValueExpr *right_result = internal_eval(context, binary_node->right);

            if (right_result->type != SBS_EXPR_VALUE_TYPE_ARRAY)
            {
                sbs_expr_free((SbsExpression*) left_result);
                sbs_expr_free((SbsExpression*) right_result);
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

                    case SBS_EXPR_VALUE_TYPE_STRING:
                        in_array = flm_cstring_equals(item->value.s, left_result->value.s);
                        break;

                    default: break;
                }
            }

            sbs_expr_free((SbsExpression*) left_result);
            sbs_expr_free((SbsExpression*) right_result);

            bin_result->value.b = in_array;

            return bin_result;
        }

        default: break;
    }

    return false;
}

static SbsValueExpr* eval_if_node(SbsIfExpr *if_node, SbsEvalContext *context)
{
    SbsValueExpr *cond_result = internal_eval(context, if_node->condition);

    SbsValueExpr *result = NULL;
    if (cond_result->type == SBS_EXPR_VALUE_TYPE_BOOL && cond_result->value.b)
        result = internal_eval(context, if_node->then_branch);
    else
        result = internal_eval(context, if_node->else_branch);

    sbs_expr_free((SbsExpression*) cond_result);

    return result;
}

static SbsValueExpr* eval_string_node(SbsStringExpr *str_node, SbsEvalContext *context)
{
    SbsValueExpr *str_value = sbs_expr_make_value(SBS_EXPR_VALUE_TYPE_STRING);

    if (str_node->is_constant)
    {
        str_value->value.s = fl_cstring_dup(str_node->format);
        return str_value;
    }

    size_t format_length = strlen(str_node->format);
    size_t format_index = 0;

    char *interpolated_string = fl_cstring_new(0);

    for (size_t i = 0; i < fl_array_length(str_node->args); i++)
    {
        // TODO: Change the type if in the future we allow other type of placeholders
        SbsStringPlaceholder *placeholder = str_node->args[i];
        
        // Copy all the characters that are part of the format string up to reach
        // the placeholder's index
        for (; format_index < placeholder->index; format_index++)
        {
            fl_cstring_append_char(&interpolated_string, str_node->format[format_index]);
        }

        // At this offset, we need to place our interpolated value
        SbsValueExpr *value = sbs_eval_expr(context, (SbsExpression*) placeholder->variable);

        if (value == NULL)
        {
            fl_cstring_append(&interpolated_string, "(null)");
        }
        else
        {
            switch (value->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    fl_cstring_vappend(&interpolated_string, "%s", (value->value.b ? "true" : "false"));
                    break;

                case SBS_EXPR_VALUE_TYPE_STRING:
                    fl_cstring_vappend(&interpolated_string, "%s", value->value.s);
                    break;

                case SBS_EXPR_VALUE_TYPE_ARRAY:
                    // TODO: Implement arrays?
                    // for (size_t k = 0; k < fl_array_length(value->value.a); k++)
                    // {
                    //     SbsExpression *item = value->value.a[i];
                    //     
                    // }
                    fl_cstring_append(&interpolated_string, "(array)");
                    break;

                default:
                    fl_cstring_append(&interpolated_string, "(unk)");
                    break;
            }
            sbs_expr_free((SbsExpression*) value);
        }
    }

    if (format_index < format_length)
    {
        for (size_t i=format_index; i < format_length; i++)
            fl_cstring_append_char(&interpolated_string, str_node->format[i]);
    }

    str_value->value.s = interpolated_string;

    return str_value;
}

static SbsValueExpr* eval_identifier_node(SbsIdentifierExpr *identifier_node, SbsEvalContext *context)
{
    // TODO: Is it correct to set STRING?
    // NOTE: STRING for compatibility with the current implementation
    SbsValueExpr *id_value = sbs_expr_make_value(SBS_EXPR_VALUE_TYPE_STRING);

    id_value->value.s = fl_cstring_dup(identifier_node->name);

    return id_value;
}

static SbsValueExpr* eval_var_definition_node(SbsVarDefinitionExpr *var_def, SbsEvalContext *context)
{
    // TODO: By now, the variables hashtable contains plain C strings instead of 
    // SbsValueExpr* objects
    char *str = sbs_eval_expr_to_cstring(context, var_def->value);

    if (str != NULL)
        fl_hashtable_add(context->variables, var_def->name->fqn, str);

    // NOTE: The hashtable copies the string. If we want to support multiple assignments, 
    // we need to return a SbsValueExpr object containing the C string instead of freeing it
    fl_cstring_free(str);
    return NULL;
}

static SbsValueExpr* internal_eval(SbsEvalContext *context, SbsExpression *node)
{
    SbsValueExpr *result = NULL;

    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            result = eval_value_node((SbsValueExpr*) node, context);
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

        case SBS_EXPR_IF:
            result = eval_if_node((SbsIfExpr*) node, context);
            break;

        case SBS_EXPR_STRING:
            result = eval_string_node((SbsStringExpr*) node, context);
            break;

        case SBS_EXPR_IDENTIFIER:
            result = eval_identifier_node((SbsIdentifierExpr*) node, context);
            break;

        case SBS_EXPR_VAR_DEFINITION:
            result = eval_var_definition_node((SbsVarDefinitionExpr*) node, context);
            break;

        default: break;
    }

    return result;
}

SbsValueExpr* sbs_eval_expr(SbsEvalContext *context, SbsExpression *node)
{
    SbsExpression *node_copy = sbs_expr_copy(node);

    SbsValueExpr *result = internal_eval(context, node_copy);

    sbs_expr_free(node_copy);

    return result;
}

bool sbs_eval_expr_to_bool(SbsEvalContext *context, SbsExpression *node)
{
    SbsExpression *node_copy = sbs_expr_copy(node);

    SbsValueExpr *result = internal_eval(context, node_copy);

    bool b = false;
    
    if (result != NULL)
    {
        if (result->type == SBS_EXPR_VALUE_TYPE_BOOL)
            b = result->value.b;

        sbs_expr_free((SbsExpression*) result);
    }

    sbs_expr_free(node_copy);

    return b;
}

static char* value_expression_to_cstring(struct SbsEvalContext *context, SbsValueExpr *value_expr)
{
    if (value_expr == NULL || value_expr->type == SBS_EXPR_VALUE_TYPE_UNK)
        return NULL;

    char *str = NULL;

    if (value_expr->type == SBS_EXPR_VALUE_TYPE_STRING)
    {
        str = value_expr->value.s != NULL ? fl_cstring_dup(value_expr->value.s) : NULL;
    }
    else if (value_expr->type == SBS_EXPR_VALUE_TYPE_BOOL)
    {
        str = value_expr->value.b ? fl_cstring_dup("true") : fl_cstring_dup("false");
    }
    else if (value_expr->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        str = fl_cstring_dup("[");
        size_t length = fl_array_length(value_expr->value.a);
        for (size_t i=0; i < length; i++)
        {
            char *itemstr = sbs_eval_expr_to_cstring(context, value_expr->value.a[i]);
            fl_cstring_vappend(&str, "%s%s", itemstr, (i == length-1 ? "" : ","));
            fl_cstring_free(itemstr);
        }
        fl_cstring_append(&str, "]");
    }

    return str;
}

char* sbs_eval_expr_to_cstring(struct SbsEvalContext *context, SbsExpression *node)
{
    SbsExpression *node_copy = sbs_expr_copy(node);

    SbsValueExpr *result = internal_eval(context, node_copy);

    char *str = NULL;
    
    if (result != NULL)
    {
        str = value_expression_to_cstring(context, result);
        sbs_expr_free((SbsExpression*) result);
    }

    sbs_expr_free(node_copy);

    return str;
}

char* sbs_eval_expr_string_to_cstring(SbsEvalContext *context, SbsStringExpr *string)
{
    return sbs_eval_expr_to_cstring(context, (SbsExpression*) string);
}
