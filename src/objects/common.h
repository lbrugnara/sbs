#ifndef SBS_OBJECT_COMMON_H
#define SBS_OBJECT_COMMON_H

#include <fllib.h>
#include "parser.h"

struct SbsStringOrId {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
};

void sbs_common_free_string(FlByte *obj);
void sbs_common_free_string_or_id(FlByte *obj);

char* sbs_common_parse_string(struct SbsParser *parser);
char* sbs_common_parse_command_string(struct SbsParser *parser);
char* sbs_common_parse_identifier(struct SbsParser *parser);
struct SbsStringOrId* sbs_common_parse_command_string_or_id_array(struct SbsParser *parser);
struct SbsStringOrId* sbs_common_parse_string_or_id_array(struct SbsParser *parser);
char** sbs_common_parse_string_array(struct SbsParser *parser);
char** sbs_common_parse_extends_declaration(struct SbsParser *parser);
char** sbs_common_parse_for_declaration(struct SbsParser *parser);
FlArray sbs_common_extend_array(FlArray dest, FlArray src);

#endif /* SBS_OBJECT_COMMON_H */
