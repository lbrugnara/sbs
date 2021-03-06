#ifndef SBS_LANG_PARSER_H
#define SBS_LANG_PARSER_H

#include "file.h"
#include "lexer.h"

typedef struct SbsParser {
    const char *filename;
    const SbsToken *tokens;
    unsigned int index;
    size_t length;
    bool has_errors;
} SbsParser;

void sbs_parser_error(SbsParser *parser, const SbsToken *token, const char *message);
void sbs_parser_warning(SbsParser *parser, const SbsToken *token, const char *message);
void sbs_parser_sync(SbsParser *parser, SbsTokenType *types, size_t length);
const SbsToken* sbs_parser_peek(SbsParser *parser);
const SbsToken* sbs_parser_peek_at(SbsParser *parser, size_t offset);
struct FlSlice sbs_parser_peek_many(SbsParser *parser, size_t n);
bool sbs_parser_next_is(SbsParser *parser, SbsTokenType type);
bool sbs_parser_next_is_not(SbsParser *parser, SbsTokenType type);
const SbsToken* sbs_parser_consume(SbsParser *parser, SbsTokenType type);
void sbs_parser_consume_if(SbsParser *parser, SbsTokenType type);
bool sbs_parser_has_input(SbsParser *parser);
bool sbs_parser_parse(SbsLexer *lexer, SbsFile *file);

#endif /* SBS_LANG_PARSER_H */
