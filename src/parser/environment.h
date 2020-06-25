#ifndef SBS_PARSER_ENVIRONMENT_H
#define SBS_PARSER_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

typedef struct {
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    SbsActionsNode actions;
} SbsEnvSection;

SbsEnvSection* sbs_env_section_parse(SbsParser *parser);
void sbs_env_section_free(SbsEnvSection *env);

#endif /* SBS_PARSER_ENVIRONMENT_H */
