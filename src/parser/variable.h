#ifndef SBS_PARSER_VARIABLE_H
#define SBS_PARSER_VARIABLE_H

#include <fllib.h>
#include "../objects/types.h"
#include "parser.h"

SbsVariable* sbs_parse_variable(SbsParser *parser);
SbsVariable** sbs_parse_variable_array(SbsParser *parser);

#endif /* SBS_PARSER_VARIABLE_H */
