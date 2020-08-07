#ifndef SBS_LANG_VARIABLE_H
#define SBS_LANG_VARIABLE_H

#include "expression.h"

typedef enum SbsVariableType {
    SBS_VARIABLE_TYPE_UNK,
    SBS_VARIABLE_TYPE_STRING,
} SbsVariableType;

typedef struct SbsNodeVariableDefinition {
    SbsVariableExpr *name;
    SbsVariableType kind;
    union {
        char *s;
    } value;
} SbsNodeVariableDefinition;

SbsNodeVariableDefinition* sbs_node_variable_definition_new(void);
void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def);

#endif /* SBS_LANG_VARIABLE_H */
