#include <fllib.h>
#include "lexer.h"

#define is_number(chr) ((chr) >= 0x0030 && (chr) <= 0x0039)
#define is_alpha(chr) (((chr) >= 0x0041 && (chr) <= 0x005A) || ((chr) >= 0x0061 && (chr) <= 0x007A))

const char *token_type_string[] = {
    [SBS_TOKEN_UNKNOWN] = "UNKNOWN",
    [SBS_TOKEN_EOF] = "EOF",

    [SBS_TOKEN_INCLUDE] = "INCLUDE",
    [SBS_TOKEN_ENV] = "ENV",
    [SBS_TOKEN_TOOLCHAIN] = "TOOLCHAIN",
    [SBS_TOKEN_TARGET] = "TARGET",
    [SBS_TOKEN_COMPILE] = "COMPILE",
    [SBS_TOKEN_ARCHIVE] = "ARCHIVE",
    [SBS_TOKEN_SHARED] = "SHARED",
    [SBS_TOKEN_EXECUTABLE] = "EXECUTABLE",
    [SBS_TOKEN_CONFIG] = "CONFIG",
    [SBS_TOKEN_PRESET] = "PRESET",
    [SBS_TOKEN_ACTION] = "ACTION",
    
    [SBS_TOKEN_EXTENDS] = "EXTENDS",
    [SBS_TOKEN_FOR] = "FOR",

    [SBS_TOKEN_LBRACE] = "LBRACE",
    [SBS_TOKEN_RBRACE] = "RBRACE",

    [SBS_TOKEN_LBRACKET] = "LBRACKET",
    [SBS_TOKEN_RBRACKET] = "RBRACKET",

    [SBS_TOKEN_COMMA] = "COMMA",
    [SBS_TOKEN_COLON] = "COLON",

    [SBS_TOKEN_STRING] = "STRING",
    [SBS_TOKEN_COMMAND_STRING] = "COMMAND_STRING",
    [SBS_TOKEN_NUMBER] = "NUMBER",
    [SBS_TOKEN_IDENTIFIER] = "IDENTIFIER"
};

SbsLexer sbs_lexer_new(const char *source, size_t length)
{
    return (SbsLexer) {
        .source = fl_slice_new((const FlByte*)source, 1, 0, length),
        .index = 0,
        .line = 1,
        .col = 0
    };
}

static inline bool has_input(SbsLexer *lexer)
{
    return lexer->index < lexer->source.length;
}

static inline char peek(SbsLexer *lexer)
{
    return lexer->source.sequence[lexer->index];
}

static inline char peek_at(SbsLexer *lexer, size_t index)
{
    if (lexer->index + index >= lexer->source.length)
        return '\0';

    return lexer->source.sequence[lexer->index + index];
}

static inline struct FlSlice peek_many(SbsLexer *lexer, size_t n)
{
    if (lexer->index + n >= lexer->source.length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new(lexer->source.sequence, 1, lexer->index, n);
}

static inline char consume(SbsLexer *lexer)
{
    lexer->col++;
    return lexer->source.sequence[lexer->index++];
}

static inline SbsToken create_token(SbsLexer *lexer, SbsTokenType type, size_t chars, unsigned int line, unsigned int col)
{
    return (SbsToken){ 
        .type = type,
        .value = fl_slice_new((FlByte*)lexer->source.sequence + lexer->index - 1, 1, 0, chars),
        .line = line,
        .col = col
    };
}

static inline void remove_ws_and_comments(SbsLexer *lexer)
{
    while(has_input(lexer))
    {
        char c = peek(lexer);

        // New lines
        if (c == '\n')
        {
            consume(lexer);
            lexer->line++;
            lexer->col = 0;
            continue;
        }
        
        // Check whitespace
        if (c == ' ' || c == '\t' || c == '\r')
        {
            consume(lexer);
            continue;
        }

        // Check comments
        struct FlSlice cc = peek_many(lexer, 2);
        
        if (c == '#' || (cc.sequence && fl_slice_equals_sequence(&cc, (FlByte*) "//", 2)))
        {
            // Consume comment line
            while (has_input(lexer) && peek(lexer) != '\n')
                consume(lexer);
            continue;
        }
        else if (cc.sequence && fl_slice_equals_sequence(&cc, (FlByte*) "/*", 2))
        {
            // Consume comment line
            while (has_input(lexer))
            {
                char c = consume(lexer);
                if (c == '*' && peek(lexer) == '/')
                {
                    consume(lexer);
                    break;
                }
                if (c == '\n')
                {
                    lexer->line++;
                    lexer->col = 0;
                }
            }
            continue;
        }

        break;
    }
}

SbsToken* sbs_lexer_tokenize(SbsLexer *lexer)
{
    FlVector *tokens = flm_vector_new_with(.element_size = sizeof(SbsToken), .capacity = 1000);

    while (has_input(lexer))
    {
        SbsToken token = sbs_lexer_next(lexer);

        if (token.type == SBS_TOKEN_EOF)
            break;

        if (token.type == SBS_TOKEN_UNKNOWN)
            flm_vexit(ERR_FATAL, "Unrecognized token %.*s", token.value.length, token.value.sequence);

        fl_vector_add(tokens, &token);

        remove_ws_and_comments(lexer);
    }

    SbsToken* sbstokens = fl_vector_to_array(tokens);

    fl_vector_free(tokens);

    // for (size_t i=0; i < fl_array_length(sbstokens); i++)
    //     fprintf(stdout, "%s: %.*s\n", token_type_string[sbstokens[i].type], sbstokens[i].value.length, sbstokens[i].value.sequence);

    return sbstokens;
}

SbsToken sbs_lexer_next(SbsLexer *lexer)
{
    remove_ws_and_comments(lexer);

    if (!has_input(lexer))
        return (SbsToken){ .type = SBS_TOKEN_EOF };

    while (has_input(lexer))
    {
        char c = peek(lexer);

        // Tokens
        if (c == '{')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_LBRACE, 1, lexer->line, lexer->col);
        }
        else if (c == '}')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_RBRACE, 1, lexer->line, lexer->col);
        }
        else if (c == '[')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_LBRACKET, 1, lexer->line, lexer->col);
        }
        else if (c == ']')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_RBRACKET, 1, lexer->line, lexer->col);
        }
        else if (c == ':')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_COLON, 1, lexer->line, lexer->col);
        }
        else if (c == ',')
        {
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_COMMA, 1, lexer->line, lexer->col);
        }
        else if (is_number(c))
        {
            const FlByte *number = lexer->source.sequence + lexer->index;

            consume(lexer);
            size_t digits = 1;   
            while (is_number(peek(lexer)))
            {
                consume(lexer);
                digits++;
            }

            return (SbsToken){ 
                .type = SBS_TOKEN_NUMBER,
                .value = fl_slice_new(number, 1, 0, digits),
                .line = lexer->line,
                .col = lexer->col
            };
        }
        else if (c == '"' || c == '\'')
        {
            c = consume(lexer);

            const FlByte *string = lexer->source.sequence + lexer->index;

            size_t chars = 0;
            while (peek(lexer) != c)
            {
                if (peek(lexer) == '\\')
                {
                    consume(lexer); // '\\'
                    chars++;

                    if (peek(lexer) == '\n')
                    {
                        consume(lexer); // '\n'
                        chars++;
                    }
                    else if (peek(lexer) == '\r' && peek_at(lexer, 1) == '\n')
                    {
                        consume(lexer); // '\r'
                        consume(lexer); // '\n'
                        chars += 2;
                    }
                }

                consume(lexer);
                chars++;
            }

            SbsToken token = { 
                .type = SBS_TOKEN_STRING,
                .value = fl_slice_new(string, 1, 0, chars),
                .line = lexer->line,
                .col = lexer->col
            };

            consume(lexer);

            return token;
        }
        else if (c == '`')
        {
            c = consume(lexer);

            const FlByte *string = lexer->source.sequence + lexer->index;

            size_t chars = 0;
            while (peek(lexer) != c)
            {
                if (peek(lexer) == '\\')
                {
                    consume(lexer); // '\\'
                    chars++;

                    if (peek(lexer) == '\n')
                    {
                        consume(lexer); // '\n'
                        chars++;
                    }
                    else if (peek(lexer) == '\r' && peek_at(lexer, 1) == '\n')
                    {
                        consume(lexer); // '\r'
                        consume(lexer); // '\n'
                        chars += 2;
                    }
                }

                consume(lexer);
                chars++;
            }

            SbsToken token = { 
                .type = SBS_TOKEN_COMMAND_STRING,
                .value = fl_slice_new(string, 1, 0, chars),
                .line = lexer->line,
                .col = lexer->col
            };

            consume(lexer);

            return token;
        }
        else if (is_alpha(c))
        {
            const FlByte *identifier = lexer->source.sequence + lexer->index;

            consume(lexer);
            size_t chars = 1;
            while (is_number(peek(lexer)) || is_alpha(peek(lexer)) || peek(lexer) == '_' || peek(lexer) == '-')
            {
                consume(lexer);
                chars++;
            }

            struct FlSlice value = fl_slice_new(identifier, 1, 0, chars);
            SbsTokenType type = SBS_TOKEN_IDENTIFIER;

            if (fl_slice_equals_sequence(&value, (FlByte*)"include", 7))
                type = SBS_TOKEN_INCLUDE;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"env", 3))
                type = SBS_TOKEN_ENV;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"toolchain", 9))
                type = SBS_TOKEN_TOOLCHAIN;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"target", 6))
                type = SBS_TOKEN_TARGET;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"compile", 7))
                type = SBS_TOKEN_COMPILE;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"archive", 7))
                type = SBS_TOKEN_ARCHIVE;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"shared", 6))
                type = SBS_TOKEN_SHARED;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"executable", 10))
                type = SBS_TOKEN_EXECUTABLE;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"config", 6))
                type = SBS_TOKEN_CONFIG;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"action", 6))
                type = SBS_TOKEN_ACTION;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"preset", 6))
                type = SBS_TOKEN_PRESET;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"extends", 7))
                type = SBS_TOKEN_EXTENDS;
            else if (fl_slice_equals_sequence(&value, (FlByte*)"for", 3))
                type = SBS_TOKEN_FOR;

            SbsToken token = { 
                .type = type,
                .value = value,
                .line = lexer->line,
                .col = lexer->col
            };

            return token;
        }
    }

    return (SbsToken){ .type = SBS_TOKEN_UNKNOWN };
}
