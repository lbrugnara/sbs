#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expression.h"

static SbsExprOperator UnaryOperators[] = {
    SBS_EXPR_OP_ID,
    SBS_EXPR_OP_NOT,
};

static SbsExprOperator BinaryOperators[] = {
    SBS_EXPR_OP_OR,
    SBS_EXPR_OP_AND,
    SBS_EXPR_OP_EQ,
    SBS_EXPR_OP_NEQ,
    SBS_EXPR_OP_IN,
};

static void free_value_node(SbsValueExpr *value)
{
    if (value->type == SBS_EXPR_VALUE_TYPE_STRING)
    {
        fl_cstring_free(value->value.s);
    }
    else if (value->type == SBS_EXPR_VALUE_TYPE_ARRAY)
    {
        fl_array_free_each_pointer(value->value.a, (FlArrayFreeElementFunc) sbs_expression_free);
    }

    fl_free(value);
}

static void free_variable_node(SbsVariableExpr *var_node)
{
    if (var_node->name)
        fl_cstring_free(var_node->name);

    if (var_node->namespace)
        fl_cstring_free(var_node->namespace);

    if (var_node->fqn)
        fl_cstring_free(var_node->fqn);

    fl_free(var_node);
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

static void free_if_node(SbsIfExpr *if_node)
{
    sbs_expression_free(if_node->condition);
    sbs_expression_free(if_node->then_branch);
    sbs_expression_free(if_node->else_branch);
    fl_free(if_node);
}

static void free_string_node(SbsStringExpr *str_expr)
{
    sbs_string_free(str_expr->value);
    fl_free(str_expr);
}

static void free_identifier_node(SbsIdentifierExpr *identifier)
{
    fl_cstring_free(identifier->name);
    fl_free(identifier);
}

static void free_var_definition_node(SbsVarDefinitionExpr *var_def)
{
    if (var_def->name)
        sbs_expression_free((SbsExpression*) var_def->name);

    if (var_def->value)
        sbs_expression_free(var_def->value);

    fl_free(var_def);
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

        case SBS_EXPR_IF:
            free_if_node((SbsIfExpr*) node);
            break;

        case SBS_EXPR_STRING:
            free_string_node((SbsStringExpr*) node);
            break;

        case SBS_EXPR_IDENTIFIER:
            free_identifier_node((SbsIdentifierExpr*) node);
            break;

        case SBS_EXPR_VAR_DEFINITION:
            free_var_definition_node((SbsVarDefinitionExpr*) node);
            break;

        default:
            return;
    }
}

static SbsExpression* copy_value_node(SbsValueExpr* node)
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
            SbsExpression *item_copy = sbs_expression_copy(node->value.a[i]);
            copy->value.a = fl_array_append(copy->value.a, &item_copy);
        }
    }

    return (SbsExpression*) copy;
}

static SbsExpression* copy_variable_node(SbsVariableExpr* node)
{
    SbsVariableExpr *copy = fl_malloc(sizeof(SbsVariableExpr));

    copy->kind = SBS_EXPR_VARIABLE;
    copy->name = fl_cstring_dup(node->name);
    copy->namespace = node->namespace != NULL ? fl_cstring_dup(node->namespace) : NULL;
    copy->fqn = fl_cstring_dup(node->fqn);

    return (SbsExpression*) copy;
}

static SbsExpression* copy_array_node(SbsArrayExpr* node)
{
    SbsArrayExpr *copy = fl_malloc(sizeof(SbsArrayExpr));

    copy->kind = SBS_EXPR_ARRAY;
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

    copy->kind = SBS_EXPR_UNARY;
    copy->op = node->op;
    copy->node = sbs_expression_copy(node->node);

    return (SbsExpression*) copy;
}

static SbsExpression* copy_binary_node(SbsBinaryExpr *node)
{
    SbsBinaryExpr *copy = fl_malloc(sizeof(SbsBinaryExpr));

    copy->kind = SBS_EXPR_BINARY;
    copy->op = node->op;
    copy->left = sbs_expression_copy(node->left);
    copy->right = sbs_expression_copy(node->right);

    return (SbsExpression*) copy;
}

static SbsExpression* copy_if_node(SbsIfExpr *if_expr)
{
    SbsIfExpr *copy = fl_malloc(sizeof(SbsIfExpr));

    copy->kind = SBS_EXPR_IF;
    copy->condition = sbs_expression_copy(if_expr->condition);
    copy->then_branch = sbs_expression_copy(if_expr->then_branch);
    copy->else_branch = sbs_expression_copy(if_expr->else_branch);

    return (SbsExpression*) copy;
}

SbsExpression* copy_string_node(SbsStringExpr *string)
{
    SbsStringExpr *copy = fl_malloc(sizeof(SbsStringExpr));

    copy->kind = SBS_EXPR_STRING;
    copy->value = sbs_string_copy(string->value);

    return (SbsExpression*) copy;
}

SbsExpression* copy_identifier_node(SbsIdentifierExpr *identifier)
{
    SbsIdentifierExpr *copy = fl_malloc(sizeof(SbsIdentifierExpr));

    copy->kind = SBS_EXPR_IDENTIFIER;
    copy->name = fl_cstring_dup(identifier->name);

    return (SbsExpression*) copy;
}

SbsExpression* copy_var_definition_node(SbsVarDefinitionExpr *var_def)
{
    SbsVarDefinitionExpr *copy = fl_malloc(sizeof(SbsVarDefinitionExpr));

    copy->kind = SBS_EXPR_VAR_DEFINITION;
    copy->name = (SbsVariableExpr*) copy_variable_node(var_def->name);
    copy->value = sbs_expression_copy(var_def->value);

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

        case SBS_EXPR_IF:
            return copy_if_node((SbsIfExpr*) node);

        case SBS_EXPR_STRING:
            return copy_string_node((SbsStringExpr*) node);

        case SBS_EXPR_IDENTIFIER:
            return copy_identifier_node((SbsIdentifierExpr*) node);

        case SBS_EXPR_VAR_DEFINITION:
            return copy_var_definition_node((SbsVarDefinitionExpr*) node);

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
    var_node->name = name;
    var_node->namespace = namespace;

    if (namespace != NULL)
    {
        var_node->fqn = fl_cstring_vdup("%s.%s", var_node->namespace, var_node->name);
    }
    else
    {
        var_node->fqn = fl_cstring_dup(var_node->name);
    }

    return var_node;
}

SbsUnaryExpr* sbs_expression_make_unary(SbsExprOperator op, SbsExpression *left)
{
    if (!fl_array_contains_n(UnaryOperators, flm_array_length(UnaryOperators), &op, sizeof(SbsExprOperator)))
        return NULL;

    SbsUnaryExpr *unary_node = fl_malloc(sizeof(SbsUnaryExpr));

    unary_node->kind = SBS_EXPR_UNARY;
    unary_node->op = op;
    unary_node->node = left;

    return unary_node;
}

SbsBinaryExpr* sbs_expression_make_binary(SbsExprOperator op, SbsExpression *left, SbsExpression *right)
{
    if (!fl_array_contains_n(BinaryOperators, flm_array_length(BinaryOperators), &op, sizeof(SbsExprOperator)))
        return NULL;

    SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));

    binode->kind = SBS_EXPR_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return binode;
}

SbsIfExpr* sbs_expression_make_if(SbsExpression *condition, SbsExpression *then_branch, SbsExpression *else_branch)
{
    SbsIfExpr *if_expr = fl_malloc(sizeof(SbsIfExpr));

    if_expr->kind = SBS_EXPR_IF;
    if_expr->condition = condition;
    if_expr->then_branch = then_branch;
    if_expr->else_branch = else_branch;

    return if_expr;
}

SbsStringExpr* sbs_expression_make_string(SbsString *string)
{
    SbsStringExpr *str_expr = fl_malloc(sizeof(SbsStringExpr));

    str_expr->kind = SBS_EXPR_STRING;
    str_expr->value = string;

    return str_expr;
}

SbsIdentifierExpr* sbs_expression_make_identifier(char *id)
{
    SbsIdentifierExpr *id_expr = fl_malloc(sizeof(SbsIdentifierExpr));

    id_expr->kind = SBS_EXPR_IDENTIFIER;
    id_expr->name = id;

    return id_expr;
}

SbsVarDefinitionExpr* sbs_expression_make_var_definition(void)
{
    SbsVarDefinitionExpr *var_def = fl_malloc(sizeof(SbsVarDefinitionExpr));
    var_def->kind = SBS_EXPR_VAR_DEFINITION;

    return var_def;
}
