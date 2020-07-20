#ifndef SBS_PARSER_ENVIRONMENT_H
#define SBS_PARSER_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

typedef struct SbsSectionEnv {
    SbsVariable *os;
    SbsVariable **arch;
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    SbsPropertyActions actions;
} SbsSectionEnv;

SbsSectionEnv* sbs_section_env_parse(SbsParser *parser);
void sbs_section_env_free(SbsSectionEnv *env);

#endif /* SBS_PARSER_ENVIRONMENT_H */
