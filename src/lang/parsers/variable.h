#ifndef SBS_PARSER_VARIABLE_H
#define SBS_PARSER_VARIABLE_H

#include "parser.h"
#include "../variable.h"

SbsNodeVariableDefinition* sbs_parse_variable_definition(SbsParser *parser);
SbsVariableExpr** sbs_parse_variable_array(SbsParser *parser);

#endif /* SBS_PARSER_VARIABLE_H */
