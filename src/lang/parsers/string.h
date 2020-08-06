#ifndef SBS_PARSER_STRING_H
#define SBS_PARSER_STRING_H

#include "../string.h"
#include "parser.h"

SbsString* sbs_string_parse(SbsParser *parser);
SbsString** sbs_string_array_parse(SbsParser *parser);

#endif /* SBS_PARSER_STRING_H */
