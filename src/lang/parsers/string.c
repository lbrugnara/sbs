#include <fllib.h>
#include "string.h"
#include "parser.h"
#include "conditional.h"
#include "variable.h"

SbsValueString* sbs_parse_interpolated_string(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_STRING);

    SbsValueStringVariablePlaceholder **placeholders = fl_array_new(sizeof(SbsValueStringVariablePlaceholder*), 0);
    char *format = fl_cstring_new(0);
    size_t format_index = 0;

    for (size_t i=0; i < token->value.length; i++)
    {
        char c = (char) token->value.sequence[i];

        if (c != '$' || i == token->value.length - 1 || (char) token->value.sequence[i + 1] != '{')
        {
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
    
    if (fl_cstring_contains(format, "\\\""))
    {
        char *tmp = fl_cstring_replace(format, "\\\"", "\"");
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

SbsValueString** sbs_parse_interpolated_string_array(SbsParser *parser)
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
            strings[index++] = sbs_parse_interpolated_string(parser);

            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return strings;
}
