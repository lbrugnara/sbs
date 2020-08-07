#ifndef SBS_LANG_ENVIRONMENT_H
#define SBS_LANG_ENVIRONMENT_H

#include "action.h"
#include "variable.h"
#include "expression.h"


typedef struct SbsSectionEnv {
    SbsExpression *condition;
    const char *name;
    const char *shell_type;
    const char *shell_command;
    char **shell_args;
    char **variables;
    SbsVariableExpr *os;
    SbsExpression *arch;
    SbsPropertyActions actions;
} SbsSectionEnv;

SbsSectionEnv* sbs_section_env_new(const struct FlSlice *name);
void sbs_section_env_free(SbsSectionEnv *env);

#endif /* SBS_LANG_ENVIRONMENT_H */
