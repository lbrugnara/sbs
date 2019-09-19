#include <fllib.h>
#include "file.h"
#include "lexer.h"
#include "parser.h"

void sbs_parser_error(const struct SbsToken *token)
{
    flm_vexit(ERR_FATAL, "Unexpected token %s %.*s in line %ld, column %ld",
        token_type_string[token->type],
        token->value.length,
        token->value.sequence,
        token->line, 
        token->col);
}

bool sbs_parser_has_input(struct SbsParser *parser)
{
    return parser->index < parser->length;
}

const struct SbsToken* sbs_parser_peek(struct SbsParser *parser)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to sbs_parser_peek\n");

    return parser->tokens + parser->index;
}

const struct SbsToken* sbs_parser_peek_at(struct SbsParser *parser, size_t offset)
{
    if (parser->index + offset >= parser->length)
        flm_vexit(ERR_FATAL, "There's no input to sbs_parser_peek at offset %zu\n", offset);

    return parser->tokens + parser->index + offset;
}

struct FlSlice sbs_parser_peek_many(struct SbsParser *parser, size_t n)
{
    if (parser->index + n >= parser->length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new((FlByte*)parser->tokens, sizeof(*parser->tokens), parser->index, n);
}

const struct SbsToken* sbs_parser_consume(struct SbsParser *parser, enum SbsTokenType type)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to sbs_parser_consume\n");

    const struct SbsToken *token = parser->tokens + parser->index++;

    if (token->type != type)
        flm_vexit(ERR_FATAL, "Unexpected token %.*s\n", token->value.length, token->value.sequence);

    return token;
}

void sbs_parser_consume_if(struct SbsParser *parser, enum SbsTokenType type)
{
    if (!sbs_parser_has_input(parser))
        return;

    if (sbs_parser_peek(parser)->type == type)
        sbs_parser_consume(parser, type);
}
