#ifndef SBS_OBJECT_PARSER_H
#define SBS_OBJECT_PARSER_H

#include "file.h"
#include "lexer.h"

struct SbsParser {
    const struct SbsToken *tokens;
    unsigned int index;
    size_t length;
};

void sbs_parser_error(const struct SbsToken *token, const char *message);
const struct SbsToken* sbs_parser_peek(struct SbsParser *parser);
const struct SbsToken* sbs_parser_peek_at(struct SbsParser *parser, size_t offset);
struct FlSlice sbs_parser_peek_many(struct SbsParser *parser, size_t n);
const struct SbsToken* sbs_parser_consume(struct SbsParser *parser, enum SbsTokenType type);
void sbs_parser_consume_if(struct SbsParser *parser, enum SbsTokenType type);
bool sbs_parser_has_input(struct SbsParser *parser);
bool sbs_parser_parse(struct SbsLexer *lexer, struct SbsFile *file);

#endif /* SBS_OBJECT_PARSER_H */
