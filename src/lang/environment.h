#ifndef SBS_LANG_ENVIRONMENT_H
#define SBS_LANG_ENVIRONMENT_H

#include "action.h"
#include "variable.h"

typedef struct SbsSectionEnv {
    SbsValueVariable *os;
    SbsValueVariable **arch;
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    SbsPropertyActions actions;
} SbsSectionEnv;

void sbs_section_env_free(SbsSectionEnv *env);

#endif /* SBS_LANG_ENVIRONMENT_H */
