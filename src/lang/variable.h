#ifndef SBS_RUNTIME_VARIABLE_H
#define SBS_RUNTIME_VARIABLE_H

#include <fllib/Slice.h>

typedef struct SbsVariableInfo {
    const char *namespace;
    const char *name;
    const char *fqn;
} SbsVariableInfo;

typedef enum SbsVariableInfoType {
    SBS_VALUE_VAR_UNK,
    SBS_VALUE_VAR_STR,
} SbsVariableInfoType;

typedef struct SbsNodeVariableDefinition {
    SbsVariableInfo *name;
    SbsVariableInfoType kind;
    union {
        char *s;
    } value;
} SbsNodeVariableDefinition;

SbsVariableInfo* sbs_varinfo_new(const char *name, const char *namespace);
SbsVariableInfo* sbs_varinfo_new_from_slice(const struct FlSlice *name, const struct FlSlice *namespace);
void sbs_varinfo_free(SbsVariableInfo *varinfo);
SbsVariableInfo* sbs_varinfo_copy(const SbsVariableInfo *varinfo);
SbsNodeVariableDefinition* sbs_node_variable_definition_new(void);
void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def);

#endif /* SBS_RUNTIME_VARIABLE_H */
