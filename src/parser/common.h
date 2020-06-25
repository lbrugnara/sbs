#ifndef SBS_PARSER_COMMON_H
#define SBS_PARSER_COMMON_H

#include <fllib.h>
#include "parser.h"

typedef struct {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
} SbsStringOrId;

#define sbs_tok_eq(slice, value_str) fl_slice_equals_sequence((slice), (FlByte*)(value_str), strlen((value_str)))

char* sbs_common_parse_string(SbsParser *parser);
char* sbs_common_parse_command_string(SbsParser *parser);
char* sbs_common_parse_identifier(SbsParser *parser);
SbsStringOrId* sbs_common_parse_command_string_or_id_array(SbsParser *parser);
SbsStringOrId* sbs_common_parse_string_or_id_array(SbsParser *parser);
char** sbs_common_parse_string_array(SbsParser *parser);
char** sbs_common_parse_extends_declaration(SbsParser *parser);
char** sbs_common_parse_for_declaration(SbsParser *parser);

#endif /* SBS_PARSER_COMMON_H */
