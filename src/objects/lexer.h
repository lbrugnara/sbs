#ifndef SBS_OBJECT_LEXER_H
#define SBS_OBJECT_LEXER_H

#include <fllib.h>

enum SbsTokenType {
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
} ;

struct SbsLexer {
    struct FlSlice source;
    unsigned int index;
    unsigned int line;
    unsigned int col;
};

struct SbsToken {
    enum SbsTokenType type;
    struct FlSlice value;
    unsigned int line;
    unsigned int col;
};

extern const char *token_type_string[];

struct SbsLexer sbs_lexer_new(const char *source, size_t length);

struct SbsToken sbs_lexer_next(struct SbsLexer *lexer);

struct SbsToken* sbs_lexer_tokenize(struct SbsLexer *lexer);

#endif /* SBS_OBJECT_LEXER_H */
