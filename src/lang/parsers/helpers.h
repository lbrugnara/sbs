#ifndef SBS_PARSER_HELPERS_H
#define SBS_PARSER_HELPERS_H

#include "parser.h"

char* sbs_parse_identifier(SbsParser *parser);
char** sbs_parse_identifier_array(SbsParser *parser);

char** sbs_parse_extends_declaration(SbsParser *parser);

#endif /* SBS_PARSER_HELPERS_H */
