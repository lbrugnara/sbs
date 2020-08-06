#ifndef SBS_PARSER_VARIABLE_H
#define SBS_PARSER_VARIABLE_H

#include "parser.h"
#include "../variable.h"

SbsVariableInfo* sbs_parse_variable(SbsParser *parser);
SbsNodeVariableDefinition* sbs_parse_variable_definition(SbsParser *parser);
SbsVariableInfo** sbs_parse_variable_array(SbsParser *parser);

#endif /* SBS_PARSER_VARIABLE_H */
