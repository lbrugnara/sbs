#ifndef SBS_PARSER_COMMAND_H
#define SBS_PARSER_COMMAND_H

#include "parser.h"
#include "../command.h"

SbsCommand* sbs_command_parse(SbsParser *parser);
SbsCommand** sbs_command_array_parse(SbsParser *parser);

#endif /* SBS_PARSER_COMMAND_H */
