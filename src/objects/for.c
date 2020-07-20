#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "for.h"
#include "../common/common.h"

void free_value_node(SbsForValueNode *value_node)
{
    if (value_node->variables)
        fl_array_free_each_pointer(value_node->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    fl_free(value_node);
}

void free_unary_node(SbsForUnaryNode *unary_node)
{
    sbs_for_node_free(unary_node->node);
    fl_free(unary_node);
}

void free_binary_node(SbsForBinaryNode *binary_node)
{
    sbs_for_node_free(binary_node->left);
    sbs_for_node_free(binary_node->right);
    fl_free(binary_node);
}

void sbs_for_node_free(SbsNodeFor *node)
{
    switch (node->kind)
    {
        case SBS_FOR_NODE_VALUE:
            free_value_node((SbsForValueNode*) node);
            break;

        case SBS_FOR_NODE_UNARY:
            free_unary_node((SbsForUnaryNode*) node);
            break;

        case SBS_FOR_NODE_BINARY:
            free_binary_node((SbsForBinaryNode*) node);
            break;

        default:
            return;
    }
}

SbsForEvalContext* sbs_for_eval_context_new(void)
{
    SbsForEvalContext *ctx = fl_malloc(sizeof(SbsForEvalContext));

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

void sbs_for_eval_context_free(SbsForEvalContext *context)
{
    fl_hashtable_free(context->variables);
    fl_free(context);
}

static bool eval_value_node(SbsForValueNode *value_node, SbsForEvalContext *context)
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

static bool eval_unary_node(SbsForUnaryNode *unary_node, SbsForEvalContext *context)
{
    bool result = sbs_for_node_eval(unary_node->node, context);

    return unary_node->op == SBS_FOR_OP_NOT ? !result : result;
}

bool eval_binary_node(SbsForBinaryNode *binary_node, SbsForEvalContext *context)
{
    bool left_result = sbs_for_node_eval(binary_node->left, context);

    switch (binary_node->op)
    {
        case SBS_FOR_OP_AND:
            // Short circuit
            return left_result && sbs_for_node_eval(binary_node->right, context);

        case SBS_FOR_OP_OR:
            // Short circuit
            return left_result || sbs_for_node_eval(binary_node->right, context);

        default:
            break;
    }

    return false;
}

bool sbs_for_node_eval(SbsNodeFor *node, SbsForEvalContext *context)
{
    switch (node->kind)
    {
        case SBS_FOR_NODE_VALUE:
            return eval_value_node((SbsForValueNode*) node, context);

        case SBS_FOR_NODE_UNARY:
            return eval_unary_node((SbsForUnaryNode*) node, context);

        case SBS_FOR_NODE_BINARY:
            return eval_binary_node((SbsForBinaryNode*) node, context);        

        default:
            return false;
    }
}


SbsNodeFor* copy_value_node(SbsForValueNode* node)
{
    SbsForValueNode *copy = fl_malloc(sizeof(SbsForValueNode));

    copy->kind = node->kind;
    copy->resource = node->resource;
    copy->variables = sbs_common_extend_array_copy(copy->variables, node->variables, (SbsArrayCopyElementFn) sbs_common_copy_string);

    return (SbsNodeFor*) copy;
}

SbsNodeFor* copy_unary_node(SbsForUnaryNode* node)
{
    SbsForUnaryNode *copy = fl_malloc(sizeof(SbsForUnaryNode));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->node = sbs_for_node_copy(node->node);

    return (SbsNodeFor*) copy;
}

SbsNodeFor* copy_binary_node(SbsForBinaryNode* node)
{
    SbsForBinaryNode *copy = fl_malloc(sizeof(SbsForBinaryNode));

    copy->kind = node->kind;
    copy->op = node->op;
    copy->left = sbs_for_node_copy(node->left);
    copy->right = sbs_for_node_copy(node->right);

    return (SbsNodeFor*) copy;
}

SbsNodeFor* sbs_for_node_copy(SbsNodeFor *node)
{
    switch (node->kind)
    {
        case SBS_FOR_NODE_VALUE:
            return copy_value_node((SbsForValueNode*) node);

        case SBS_FOR_NODE_UNARY:
            return copy_unary_node((SbsForUnaryNode*) node);

        case SBS_FOR_NODE_BINARY:
            return copy_binary_node((SbsForBinaryNode*) node);

        default:
            break;
    }

    return NULL;
}

SbsNodeFor* sbs_for_node_make_binary(SbsForOperatorKind op, SbsNodeFor *left, SbsNodeFor *right)
{
    SbsForBinaryNode *binode = fl_malloc(sizeof(SbsForBinaryNode));

    binode->kind = SBS_FOR_NODE_BINARY;
    binode->op = op;
    binode->left = left;
    binode->right = right;

    return (SbsNodeFor*) binode;
}
