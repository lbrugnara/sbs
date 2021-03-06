#include <fllib.h>
#include "lexer.h"

#define is_number(chr) ((chr) >= 0x0030 && (chr) <= 0x0039)
#define is_alpha(chr) (((chr) >= 0x0041 && (chr) <= 0x005A) || ((chr) >= 0x0061 && (chr) <= 0x007A))
#define is_alnum(chr) (is_number((chr)) || is_alpha((chr)))

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
    [SBS_TOKEN_IF] = "IF",
    [SBS_TOKEN_ELSE] = "ELSE",

    [SBS_TOKEN_OP_AND] = "AND",
    [SBS_TOKEN_OP_OR] = "OR",
    [SBS_TOKEN_OP_NOT] = "NOT",
    [SBS_TOKEN_OP_EQ] = "EQUALS",
    [SBS_TOKEN_OP_NEQ] = "NOT EQUALS",

    [SBS_TOKEN_LPAREN] = "LPAREN",
    [SBS_TOKEN_RPAREN] = "RPAREN",

    [SBS_TOKEN_LBRACE] = "LBRACE",
    [SBS_TOKEN_RBRACE] = "RBRACE",

    [SBS_TOKEN_LBRACKET] = "LBRACKET",
    [SBS_TOKEN_RBRACKET] = "RBRACKET",

    [SBS_TOKEN_COMMA] = "COMMA",
    [SBS_TOKEN_COLON] = "COLON",
    [SBS_TOKEN_ASSIGN] = "ASSIGN",
    [SBS_TOKEN_QUESTION] = "QUESTION",

    [SBS_TOKEN_STRING] = "STRING",
    [SBS_TOKEN_COMMAND_STRING] = "COMMAND_STRING",
    [SBS_TOKEN_NUMBER] = "NUMBER",
    [SBS_TOKEN_IDENTIFIER] = "IDENTIFIER",
    [SBS_TOKEN_VARIABLE] = "VARIABLE",
};

SbsLexer sbs_lexer_new(const char *source, size_t length)
{
    return (SbsLexer) {
        .source = fl_slice_new((const FlByte*)source, 1, 0, length),
        .index = 0,
        .line = 1,
        .col = 1
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
        .value = fl_slice_new((FlByte*)lexer->source.sequence + lexer->index - chars, 1, 0, chars),
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
            lexer->col = 1;
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
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_LBRACE, 1, line, col);
        }
        else if (c == '}')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_RBRACE, 1, line, col);
        }
        else if (c == '[')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_LBRACKET, 1, line, col);
        }
        else if (c == ']')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_RBRACKET, 1, line, col);
        }
        else if (c == '(')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_LPAREN, 1, line, col);
        }
        else if (c == ')')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_RPAREN, 1, line, col);
        }
        else if (c == ':')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_COLON, 1, line, col);
        }
        else if (c == ',')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_COMMA, 1, line, col);
        }
        else if (c == '?')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_QUESTION, 1, line, col);
        }
        else if (is_number(c))
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
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
                .line = line,
                .col = col
            };
        }
        else if (c == '"' || c == '\'')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
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
                .line = line,
                .col = col
            };

            consume(lexer);

            return token;
        }
        else if (c == '`')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
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
                .line = line,
                .col = col
            };

            consume(lexer);

            return token;
        }
        else if (c == '=' && peek_at(lexer, 1) == '=')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_OP_EQ, 2, line, col);
        }
        else if (c == '!' && peek_at(lexer, 1) == '=')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_OP_NEQ, 2, line, col);
        }
        else if (c == '=')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;
            consume(lexer);
            return create_token(lexer, SBS_TOKEN_ASSIGN, 1, line, col);
        }
        else if (c == '$')
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;

            consume(lexer);

            const FlByte *variable = lexer->source.sequence + lexer->index;

            size_t chars = 0;
            while (is_alnum(peek(lexer)) || peek(lexer) == '.' || peek(lexer) == '_')
            {
                consume(lexer);
                chars++;
            }

            if (chars == 0)
            {
                return (SbsToken) {
                    .type = SBS_TOKEN_UNKNOWN,
                    .value = fl_slice_new(lexer->source.sequence + lexer->index, 1, 0, 1),
                    .line = line,
                    .col = col
                };
            }

            SbsToken token = { 
                .type = SBS_TOKEN_VARIABLE,
                .value = fl_slice_new(variable, 1, 0, chars),
                .line = line,
                .col = col
            };

            return token;
        }
        else if (is_alpha(c))
        {
            unsigned int line = lexer->line;
            unsigned int col = lexer->col;

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
            {
                type = SBS_TOKEN_INCLUDE;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"env", 3))
            {
                type = SBS_TOKEN_ENV;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"toolchain", 9))
            {
                type = SBS_TOKEN_TOOLCHAIN;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"target", 6))
            {
                type = SBS_TOKEN_TARGET;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"compile", 7))
            {
                type = SBS_TOKEN_COMPILE;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"archive", 7))
            {
                type = SBS_TOKEN_ARCHIVE;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"shared", 6))
            {
                type = SBS_TOKEN_SHARED;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"executable", 10))
            {
                type = SBS_TOKEN_EXECUTABLE;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"config", 6))
            {
                type = SBS_TOKEN_CONFIG;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"action", 6))
            {
                type = SBS_TOKEN_ACTION;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"preset", 6))
            {
                type = SBS_TOKEN_PRESET;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"extends", 7))
            {
                type = SBS_TOKEN_EXTENDS;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"for", 3))
            {
                type = SBS_TOKEN_FOR;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"if", 2))
            {
                type = SBS_TOKEN_IF;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"else", 4))
            {
                type = SBS_TOKEN_ELSE;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"and", 3))
            {
                type = SBS_TOKEN_OP_AND;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"or", 2))
            {
                type = SBS_TOKEN_OP_OR;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"not", 3))
            {
                type = SBS_TOKEN_OP_NOT;
            }
            else if (fl_slice_equals_sequence(&value, (FlByte*)"in", 2))
            {
                type = SBS_TOKEN_OP_IN;
            }

            SbsToken token = { 
                .type = type,
                .value = value,
                .line = line,
                .col = col
            };

            return token;
        }
        else break;
    }

    unsigned int line = lexer->line;
    unsigned int col = lexer->col;
    consume(lexer);
    return create_token(lexer, SBS_TOKEN_UNKNOWN, 1, line, col);
}
