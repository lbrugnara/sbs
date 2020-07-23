#ifndef SBS_RUNTIME_EVAL_H
#define SBS_RUNTIME_EVAL_H

#include <fllib/containers/Hashtable.h>

typedef enum SbsEvalOperatorKind {
    SBS_EVAL_OP_UNK,
    SBS_EVAL_OP_ID,
    SBS_EVAL_OP_AND,
    SBS_EVAL_OP_OR,
    SBS_EVAL_OP_NOT
} SbsEvalOperatorKind;

typedef enum SbsEvalNodeKind {
    SBS_EVAL_NODE_UNK,
    SBS_EVAL_NODE_UNARY,
    SBS_EVAL_NODE_BINARY,
    SBS_EVAL_NODE_VALUE,
} SbsEvalNodeKind;

typedef struct SbsEvalNode {    
    SbsEvalNodeKind kind;
} SbsEvalNode;

// TODO: SbsForResourceKind shouldn't be necessary, we could simply create
// an SbsEvalVariable to collect them from the context
typedef enum SbsForResourceKind {
    SBS_FOR_RESOURCE_OS,
    SBS_FOR_RESOURCE_ARCH,
    SBS_FOR_RESOURCE_ENV,
    SBS_FOR_RESOURCE_TOOLCHAIN,
    SBS_FOR_RESOURCE_CONFIG,
    SBS_FOR_RESOURCE_TARGET,
} SbsForResourceKind;

typedef struct SbsEvalArrayNode {
    SbsEvalNodeKind kind;
    SbsForResourceKind resource;
    char **variables;
} SbsEvalArrayNode;

typedef struct SbsEvalUnaryNode {
    SbsEvalNodeKind kind;
    SbsEvalOperatorKind op;
    SbsEvalNode *node;
} SbsEvalUnaryNode;

typedef struct SbsEvalBinaryNode {
    SbsEvalNodeKind kind;
    SbsEvalOperatorKind op;
    SbsEvalNode *left;
    SbsEvalNode *right;
} SbsEvalBinaryNode;

typedef struct SbsEvalContext {
    FlHashtable *variables;
} SbsEvalContext;

SbsEvalContext* sbs_eval_context_new(void);
bool sbs_eval_run(SbsEvalNode *node, SbsEvalContext *context);
void sbs_eval_context_free(SbsEvalContext *context);

void sbs_eval_node_free(SbsEvalNode *node);
SbsEvalNode* sbs_eval_node_copy(SbsEvalNode *node);
SbsEvalNode* sbs_eval_node_make_binary(SbsEvalOperatorKind op, SbsEvalNode *left, SbsEvalNode *right);

#endif /* SBS_RUNTIME_EVAL_H */
