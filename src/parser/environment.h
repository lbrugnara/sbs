#ifndef SBS_PARSER_ENVIRONMENT_H
#define SBS_PARSER_ENVIRONMENT_H

#include <fllib.h>
#include "action.h"

struct SbsEnvSection {
    const char *name;
    const char *type;
    const char *terminal;
    char **args;
    char **variables;
    struct SbsActionsNode actions;
};

struct SbsEnvSection* sbs_env_section_parse(struct SbsParser *parser);
void sbs_env_section_free(struct SbsEnvSection *env);

#endif /* SBS_PARSER_ENVIRONMENT_H */
