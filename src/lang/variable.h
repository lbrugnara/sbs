#ifndef SBS_LANG_VARIABLE_H
#define SBS_LANG_VARIABLE_H

#include <fllib/Slice.h>

typedef struct SbsValueVariable {
    const char *namespace;
    const char *name;
    const char *fqn;
} SbsValueVariable;

typedef enum SbsValueVariableType {
    SBS_VALUE_VAR_UNK,
    SBS_VALUE_VAR_STR,
} SbsValueVariableType;

typedef struct SbsNodeVariableDefinition {
    SbsValueVariable *name;
    SbsValueVariableType kind;
    union {
        char *s;
    } value;
} SbsNodeVariableDefinition;

SbsValueVariable* sbs_value_variable_new(const struct FlSlice *name, const struct FlSlice *namespace);
void sbs_value_variable_free(SbsValueVariable *variable);
SbsValueVariable* sbs_value_variable_copy(const SbsValueVariable *variable);
SbsNodeVariableDefinition* sbs_node_variable_definition_new(void);
void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def);

#endif /* SBS_LANG_VARIABLE_H */
