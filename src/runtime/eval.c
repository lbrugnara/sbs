#include <fllib/Cstring.h>
#include "eval.h"
#include "../utils.h"

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

static bool eval_value_node(SbsEvalArrayNode *value_node, SbsEvalContext *context)
{
    const char *value = NULL;

    switch(value_node->resource)
    {
        case SBS_FOR_RESOURCE_OS:
            value = fl_hashtable_get(context->variables, "$sbs.os");
            break;

        case SBS_FOR_RESOURCE_ARCH:
            value = fl_hashtable_get(context->variables, "$sbs.arch");
            break;

        case SBS_FOR_RESOURCE_ENV:
            value = fl_hashtable_get(context->variables, "$sbs.env");
            break;

        case SBS_FOR_RESOURCE_TOOLCHAIN:
            value = fl_hashtable_get(context->variables, "$sbs.toolchain");
            break;

        case SBS_FOR_RESOURCE_CONFIG:
            value = fl_hashtable_get(context->variables, "$sbs.config");
            break;

        case SBS_FOR_RESOURCE_TARGET:
            value = fl_hashtable_get(context->variables, "$sbs.target");
            break;
    }

    if (!value)
        return false;

    for (size_t i=0; i < fl_array_length(value_node->variables); i++)
    {
        if (flm_cstring_equals(value_node->variables[i], value))
            return true;
    }

    return false;
}

static bool eval_unary_node(SbsEvalUnaryNode *unary_node, SbsEvalContext *context)
{
    bool result = sbs_eval_run(unary_node->node, context);

    return unary_node->op == SBS_EVAL_OP_NOT ? !result : result;
}

static bool eval_binary_node(SbsEvalBinaryNode *binary_node, SbsEvalContext *context)
{
    bool left_result = sbs_eval_run(binary_node->left, context);

    switch (binary_node->op)
    {
        case SBS_EVAL_OP_AND:
            // Short circuit
            return left_result && sbs_eval_run(binary_node->right, context);

        case SBS_EVAL_OP_OR:
            // Short circuit
            return left_result || sbs_eval_run(binary_node->right, context);

        default:
            break;
    }

    return false;
}

bool sbs_eval_run(SbsEvalNode *node, SbsEvalContext *context)
{
    switch (node->kind)
    {
        case SBS_EVAL_NODE_VALUE:
            return eval_value_node((SbsEvalArrayNode*) node, context);

        case SBS_EVAL_NODE_UNARY:
            return eval_unary_node((SbsEvalUnaryNode*) node, context);

        case SBS_EVAL_NODE_BINARY:
            return eval_binary_node((SbsEvalBinaryNode*) node, context);        

        default:
            return false;
    }
}

static void free_value_node(SbsEvalArrayNode *value_node)
{
    if (value_node->variables)
        fl_array_free_each_pointer(value_node->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(value_node);
}

static void free_unary_node(SbsEvalUnaryNode *unary_node)
{
    sbs_eval_node_free(unary_node->node);
    fl_free(unary_node);
}

static void free_binary_node(SbsEvalBinaryNode *binary_node)
{
    sbs_eval_node_free(binary_node->left);
    sbs_eval_node_free(binary_node->right);
    fl_free(binary_node);
}

void sbs_eval_node_free(SbsEvalNode *node)
{
    switch (node->kind)
    {
        case SBS_EVAL_NODE_VALUE:
            free_value_node((SbsEvalArrayNode*) node);
            break;

        case SBS_EVAL_NODE_UNARY:
            free_unary_node((SbsEvalUnaryNode*) node);
            break;

        case SBS_EVAL_NODE_BINARY:
            free_binary_node((SbsEvalBinaryNode*) node);
            break;

        default:
            return;
    }
}

static SbsEvalNode* copy_value_node(SbsEvalArrayNode* node)
{
    SbsEvalArrayNode *copy = fl_malloc(sizeof(SbsEvalArrayNode));

    copy->kind = node->kind;
    copy->resource = node->resource;
    copy->variables = sbs_string_array_extend(copy->variables, node->variables);

    return (SbsEvalNode*) copy;
}

static SbsEvalNode* copy_unary_node(SbsEvalUnaryNode* node)
{
    SbsEvalUnaryNode *copy = fl_malloc(sizeof(SbsEvalUnaryNode));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->node = sbs_eval_node_copy(node->node);

    return (SbsEvalNode*) copy;
}

static SbsEvalNode* copy_binary_node(SbsEvalBinaryNode* node)
{
    SbsEvalBinaryNode *copy = fl_malloc(sizeof(SbsEvalBinaryNode));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->left = sbs_eval_node_copy(node->left);
    copy->right = sbs_eval_node_copy(node->right);

    return (SbsEvalNode*) copy;
}

SbsEvalNode* sbs_eval_node_copy(SbsEvalNode *node)
{
    switch (node->kind)
    {
        case SBS_EVAL_NODE_VALUE:
            return copy_value_node((SbsEvalArrayNode*) node);

        case SBS_EVAL_NODE_UNARY:
            return copy_unary_node((SbsEvalUnaryNode*) node);

        case SBS_EVAL_NODE_BINARY:
            return copy_binary_node((SbsEvalBinaryNode*) node);

        default:
            break;
    }

    return NULL;
}

SbsEvalNode* sbs_eval_node_make_binary(SbsEvalOperatorKind op, SbsEvalNode *left, SbsEvalNode *right)
{
    SbsEvalBinaryNode *binode = fl_malloc(sizeof(SbsEvalBinaryNode));

    binode->kind = SBS_EVAL_NODE_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return (SbsEvalNode*) binode;
}
