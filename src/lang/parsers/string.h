#ifndef SBS_PARSER_STRING_H
#define SBS_PARSER_STRING_H

#include "../string.h"
#include "parser.h"

SbsValueString* sbs_parse_interpolated_string(SbsParser *parser);
SbsValueString** sbs_parse_interpolated_string_array(SbsParser *parser);

#endif /* SBS_PARSER_STRING_H */
