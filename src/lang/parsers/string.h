#ifndef SBS_PARSER_STRING_H
#define SBS_PARSER_STRING_H

#include "../string.h"
#include "parser.h"

SbsValueString* sbs_value_string_parse(SbsParser *parser);
SbsValueString** sbs_value_string_array_parse(SbsParser *parser);

#endif /* SBS_PARSER_STRING_H */
