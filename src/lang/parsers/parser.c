#include <stdio.h>
#include <fllib.h>
#include "file.h"
#include "lexer.h"
#include "parser.h"

extern const char *token_type_string[];

void sbs_parser_warning(SbsParser *parser, const SbsToken *token, const char *message)
{
    fprintf(stderr, "Token %s %.*s in line %u, column %u %s\n",
        token_type_string[token->type],
        (int) token->value.length,
        token->value.sequence,
        token->line, 
        token->col,
        message);
}

void sbs_parser_sync(SbsParser *parser, SbsTokenType *types, size_t length)
{
    do 
    {
        const SbsToken *token = sbs_parser_peek(parser);

        bool end = false;
        for (size_t i=0; i < length; i++)
        {
            if (types[i] == token->type)
            {
                end = true;
                break;
            }
        }

        if (end)
            break;

        sbs_parser_consume(parser, token->type);        
    }
    while (true);
}

void sbs_parser_error(const SbsToken *token, const char *message)
{
    flm_vexit(ERR_FATAL, "Unexpected token %s %.*s in line %ld, column %ld %s",
        token_type_string[token->type],
        token->value.length,
        token->value.sequence,
        token->line, 
        token->col,
        message);
}

bool sbs_parser_has_input(SbsParser *parser)
{
    return parser->index < parser->length;
}

const SbsToken* sbs_parser_peek(SbsParser *parser)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to sbs_parser_peek\n");

    return parser->tokens + parser->index;
}

const SbsToken* sbs_parser_peek_at(SbsParser *parser, size_t offset)
{
    if (parser->index + offset >= parser->length)
        flm_vexit(ERR_FATAL, "There's no input to sbs_parser_peek at offset %zu\n", offset);

    return parser->tokens + parser->index + offset;
}

struct FlSlice sbs_parser_peek_many(SbsParser *parser, size_t n)
{
    if (parser->index + n >= parser->length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new((FlByte*)parser->tokens, sizeof(*parser->tokens), parser->index, n);
}

const SbsToken* sbs_parser_consume(SbsParser *parser, SbsTokenType type)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to sbs_parser_consume\n");

    const SbsToken *token = parser->tokens + parser->index++;

    if (token->type != type)
    {
        char *message = fl_cstring_vdup("expecting %s but received %s", token_type_string[(size_t)type], token_type_string[(size_t)token->type]);
        sbs_parser_error(token, message);
        fl_cstring_free(message);
        return NULL;
    }

    return token;
}

void sbs_parser_consume_if(SbsParser *parser, SbsTokenType type)
{
    if (!sbs_parser_has_input(parser))
        return;

    if (sbs_parser_peek(parser)->type == type)
        sbs_parser_consume(parser, type);
}
