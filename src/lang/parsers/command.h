#ifndef SBS_PARSER_COMMAND_H
#define SBS_PARSER_COMMAND_H

#include "parser.h"
#include "../command.h"

SbsString* sbs_value_command_string_parse(SbsParser *parser);
SbsValueCommand** sbs_value_command_array_parse(SbsParser *parser);

#endif /* SBS_PARSER_COMMAND_H */
