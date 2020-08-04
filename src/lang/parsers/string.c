#include <fllib.h>
#include "string.h"
#include "parser.h"
#include "conditional.h"
#include "variable.h"

SbsValueString* sbs_value_string_parse(SbsParser *parser)
{
    SbsTokenType token_type = sbs_parser_next_is(parser, SBS_TOKEN_STRING)
                            ? SBS_TOKEN_STRING
                            : sbs_parser_next_is(parser, SBS_TOKEN_COMMAND_STRING)
                                ? SBS_TOKEN_COMMAND_STRING
                                : SBS_TOKEN_UNKNOWN;

    if (token_type == SBS_TOKEN_UNKNOWN)
    {
        sbs_parser_warning(parser, sbs_parser_peek(parser), "Expecting a string or command string");
        return NULL;
    }

    const SbsToken *token = sbs_parser_consume(parser, token_type);

    SbsValueStringVariablePlaceholder **placeholders = fl_array_new(sizeof(SbsValueStringVariablePlaceholder*), 0);
    char *format = fl_cstring_new(0);
    size_t format_index = 0;

    for (size_t i=0; i < token->value.length; i++)
    {
        char c = (char) token->value.sequence[i];

        if (c != '$' || i == token->value.length - 1 || (char) token->value.sequence[i + 1] != '{')
        {
            if (c == '\\')
            {
                // If it is a \ followed by a newline, the string continues in the next line, so we 
                // consume both '\' and '\n'
                if (i < token->value.length - 1 && (char) token->value.sequence[i + 1] == '\n')
                {
                    i++; // consume the '\' and let the for increment consume the '\n'
                    continue;
                }
                else if (i < token->value.length - 2 && (char) token->value.sequence[i + 1] == '\r' && (char) token->value.sequence[i + 2] == '\n')
                {
                    i++; // consume the '\'
                    i++; // consume the '\r' and let the for increment consume the '\n'
                    continue;
                }
            }

            format_index++;
            fl_cstring_append_char(&format, c);
            continue;
        }

        SbsValueStringVariablePlaceholder *placeholder = fl_malloc(sizeof(SbsValueStringVariablePlaceholder));
        placeholder->index = format_index;

        i++; // consume the '$'
        i++; // consume the '{'. There's no need to consume the '}', it will be skipped in the loop body

        bool is_valid = false;
        // We assume the variable has a namespace that starts at i
        size_t ns_index = i;
        size_t ns_length = 0;
        size_t name_index = 0;
        size_t name_length = 0;
        for (size_t j=i; j < token->value.length; j++)
        {
            char c = (char) token->value.sequence[j];

            if (c == '}')
            {
                // Update the index to point to '}', the next increment will skip it
                i = j;
                // If we didn't find a '.', it is a name, not a namespace
                if (name_index == 0 && name_length == 0)
                {
                    name_index = ns_index;
                    name_length = ns_length;
                    ns_index = 0;
                    ns_length = 0;
                }
                is_valid = true;
                break;
            }

            if (name_index == 0 && c == '.')
            {
                ns_length = j - i;
                name_index = j + 1;
            }
            else if (name_index != 0)
            {
                name_length++;
            }
            else
            {
                ns_length++;
            }
        }

        if (!is_valid)
        {
            sbs_parser_warning(parser, token, "Invalid string interpolation");
            fl_free(placeholder);
            continue;
        }

        struct FlSlice name = fl_slice_new(token->value.sequence, 1, name_index, name_length);
        if (ns_length > 0)
        {
            struct FlSlice namespace = fl_slice_new(token->value.sequence, 1, ns_index, ns_length);
            placeholder->variable = sbs_value_variable_new(&name, &namespace);
        }
        else
        {
            placeholder->variable = sbs_value_variable_new(&name, NULL);
        }

        placeholders = fl_array_append(placeholders, &placeholder);
    }
    
    const char *escape = token_type == SBS_TOKEN_STRING ? "\\\"" : "\\`";
    const char *escape_repl = token_type == SBS_TOKEN_STRING ? "\"" : "`";
    if (fl_cstring_contains(format, escape))
    {
        char *tmp = fl_cstring_replace(format, escape, escape_repl);
        if (!tmp)
            return NULL;
        fl_cstring_free(format);
        format = tmp;
    }

    SbsValueString *string = fl_malloc(sizeof(SbsValueString));
    string->format = format;
    string->args = placeholders;

    return string;
}

SbsValueString** sbs_value_string_array_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many strings
    size_t strings_count = 0;
    const SbsToken *tmp;

    for (size_t i=0; (tmp = sbs_parser_peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        strings_count++;
    }

    SbsValueString **strings = NULL;

    if (strings_count != 0)
    {
        // Parse the strings
        strings = fl_array_new(sizeof(SbsValueString*), strings_count);
        size_t index = 0;

        while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            strings[index++] = sbs_value_string_parse(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return strings;
}
