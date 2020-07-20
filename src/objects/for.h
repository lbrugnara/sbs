#ifndef SBS_OBJECTS_FOR_H
#define SBS_OBJECTS_FOR_H

#include <fllib/containers/Hashtable.h>

typedef enum SbsForResourceKind {
    SBS_FOR_RESOURCE_OS,
    SBS_FOR_RESOURCE_ARCH,
    SBS_FOR_RESOURCE_ENV,
    SBS_FOR_RESOURCE_TOOLCHAIN,
    SBS_FOR_RESOURCE_CONFIG,
    SBS_FOR_RESOURCE_TARGET,
} SbsForResourceKind;

typedef enum SbsForOperatorKind {
    SBS_FOR_OP_UNK,
    SBS_FOR_OP_ID,
    SBS_FOR_OP_AND,
    SBS_FOR_OP_OR,
    SBS_FOR_OP_NOT
} SbsForOperatorKind;

typedef enum SbsForNodeKind {
    SBS_FOR_NODE_UNK,
    SBS_FOR_NODE_UNARY,
    SBS_FOR_NODE_BINARY,
    SBS_FOR_NODE_VALUE,
} SbsForNodeKind;

typedef struct SbsNodeFor {    
    SbsForNodeKind kind;
} SbsNodeFor;

typedef struct SbsForValueNode {
    SbsForNodeKind kind;
    SbsForResourceKind resource;
    char **variables;
} SbsForValueNode;

typedef struct SbsForUnaryNode {
    SbsForNodeKind kind;
    SbsForOperatorKind op;
    SbsNodeFor *node;
} SbsForUnaryNode;

typedef struct SbsForBinaryNode {
    SbsForNodeKind kind;
    SbsForOperatorKind op;
    SbsNodeFor *left;
    SbsNodeFor *right;
} SbsForBinaryNode;

typedef struct SbsForEvalContext {
    FlHashtable *variables;
} SbsForEvalContext;

SbsForEvalContext* sbs_for_eval_context_new(void);
void sbs_for_eval_context_free(SbsForEvalContext *context);
bool sbs_for_node_eval(SbsNodeFor *node, SbsForEvalContext *context);
void sbs_for_node_free(SbsNodeFor *node);
SbsNodeFor* sbs_for_node_copy(SbsNodeFor *node);
SbsNodeFor* sbs_for_node_make_binary(SbsForOperatorKind op, SbsNodeFor *left, SbsNodeFor *right);

#endif /* SBS_OBJECTS_FOR_H */
