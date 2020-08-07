#ifndef SBS_LANG_VARIABLE_H
#define SBS_LANG_VARIABLE_H

#include <fllib/Slice.h>

typedef struct SbsVariable {
    const char *namespace;
    const char *name;
    const char *fqn;
} SbsVariable;

typedef enum SbsVariableType {
    SBS_VARIABLE_TYPE_UNK,
    SBS_VARIABLE_TYPE_STRING,
} SbsVariableType;

typedef struct SbsNodeVariableDefinition {
    SbsVariable *name;
    SbsVariableType kind;
    union {
        char *s;
    } value;
} SbsNodeVariableDefinition;

SbsVariable* sbs_variable_new(const char *name, const char *namespace);
SbsVariable* sbs_variable_new_from_slice(const struct FlSlice *name, const struct FlSlice *namespace);
void sbs_variable_free(SbsVariable *varinfo);
SbsVariable* sbs_variable_copy(const SbsVariable *varinfo);
SbsNodeVariableDefinition* sbs_node_variable_definition_new(void);
void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def);

#endif /* SBS_LANG_VARIABLE_H */
