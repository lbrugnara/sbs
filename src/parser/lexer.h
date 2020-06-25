#ifndef SBS_PARSER_LEXER_H
#define SBS_PARSER_LEXER_H

#include <fllib.h>

typedef enum {
    SBS_TOKEN_UNKNOWN,
    SBS_TOKEN_EOF,

    SBS_TOKEN_INCLUDE,
    SBS_TOKEN_ENV,
    SBS_TOKEN_TOOLCHAIN,
    SBS_TOKEN_TARGET,
    SBS_TOKEN_COMPILE,
    SBS_TOKEN_ARCHIVE,
    SBS_TOKEN_SHARED,
    SBS_TOKEN_EXECUTABLE,
    SBS_TOKEN_CONFIG,
    SBS_TOKEN_PRESET,
    SBS_TOKEN_ACTION,
    
    SBS_TOKEN_EXTENDS,
    SBS_TOKEN_FOR,

    SBS_TOKEN_LBRACE,
    SBS_TOKEN_RBRACE,

    SBS_TOKEN_LBRACKET,
    SBS_TOKEN_RBRACKET,

    SBS_TOKEN_COMMA,
    SBS_TOKEN_COLON,

    SBS_TOKEN_STRING,
    SBS_TOKEN_COMMAND_STRING,
    SBS_TOKEN_NUMBER,
    SBS_TOKEN_IDENTIFIER
} SbsTokenType;

typedef struct {
    struct FlSlice source;
    unsigned int index;
    unsigned int line;
    unsigned int col;
} SbsLexer;

typedef struct {
    SbsTokenType type;
    struct FlSlice value;
    unsigned int line;
    unsigned int col;
} SbsToken;

extern const char *token_type_string[];

SbsLexer sbs_lexer_new(const char *source, size_t length);

SbsToken sbs_lexer_next(SbsLexer *lexer);

SbsToken* sbs_lexer_tokenize(SbsLexer *lexer);

#endif /* SBS_PARSER_LEXER_H */
