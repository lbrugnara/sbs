#ifndef SBS_PARSER_HELPERS_H
#define SBS_PARSER_HELPERS_H

#include <fllib.h>
#include "../objects/types.h"
#include "parser.h"

char* sbs_parse_string(SbsParser *parser);
char** sbs_parse_string_array(SbsParser *parser);

SbsStringOrId* sbs_parse_string_or_id_array(SbsParser *parser);

char* sbs_parse_command_string(SbsParser *parser);

SbsStringOrId* sbs_parse_command_string_or_id_array(SbsParser *parser);

char* sbs_parse_identifier(SbsParser *parser);
char** sbs_parse_identifier_array(SbsParser *parser);

char** sbs_parse_extends_declaration(SbsParser *parser);

#endif /* SBS_PARSER_HELPERS_H */
