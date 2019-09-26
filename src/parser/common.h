#ifndef SBS_PARSER_COMMON_H
#define SBS_PARSER_COMMON_H

#include <fllib.h>
#include "parser.h"

struct SbsStringOrId {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
};

char* sbs_common_parse_string(struct SbsParser *parser);
char* sbs_common_parse_command_string(struct SbsParser *parser);
char* sbs_common_parse_identifier(struct SbsParser *parser);
struct SbsStringOrId* sbs_common_parse_command_string_or_id_array(struct SbsParser *parser);
struct SbsStringOrId* sbs_common_parse_string_or_id_array(struct SbsParser *parser);
char** sbs_common_parse_string_array(struct SbsParser *parser);
char** sbs_common_parse_extends_declaration(struct SbsParser *parser);
char** sbs_common_parse_for_declaration(struct SbsParser *parser);

#endif /* SBS_PARSER_COMMON_H */
