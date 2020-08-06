#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "string.h"
#include "../runtime/eval.h"

SbsString* sbs_string_new(char *format, bool is_constant)
{
    SbsString *string = fl_malloc(sizeof(SbsString));

    string->format = format;
    string->is_constant = is_constant;

    if (!is_constant)
        string->args = fl_array_new(sizeof(SbsStringPlaceholder*), 0);

    return string;
}

char* sbs_string_interpolate(SbsEvalContext *context, SbsString *string)
{
    if (string->is_constant)
        return fl_cstring_dup(string->format);        

    size_t format_length = strlen(string->format);
    size_t format_index = 0;

    char *interpolated_string = fl_cstring_new(0);

    for (size_t i = 0; i < fl_array_length(string->args); i++)
    {
        // TODO: Change the type if in the future we allow other type of placeholders
        SbsStringPlaceholder *placeholder = string->args[i];
        
        // Copy all the characters that are part of the format string up to reach
        // the placeholder's index
        for (; format_index < placeholder->index; format_index++)
        {
            fl_cstring_append_char(&interpolated_string, string->format[format_index]);
        }

        // At this offset, we need to place our interpolated value
        // TODO: If we support other type of expressions, we need to update this
        SbsExpression *var_expr = (SbsExpression*) sbs_expression_make_variable(placeholder->variable->name, placeholder->variable->namespace);
        SbsValueExpr *value = sbs_expression_eval(context, var_expr);

        if (value == NULL)
        {
            fl_cstring_append(&interpolated_string, "(null)");
        }
        else
        {
            switch (value->type)
            {
                case SBS_EXPR_VALUE_TYPE_BOOL:
                    fl_cstring_vappend(&interpolated_string, "%s", (value->value.b ? "true" : "false"));
                    break;

                case SBS_EXPR_VALUE_TYPE_STRING:
                    fl_cstring_vappend(&interpolated_string, "%s", value->value.s);
                    break;

                case SBS_EXPR_VALUE_TYPE_ARRAY:
                    // TODO: Implement arrays?
                    // for (size_t k = 0; k < fl_array_length(value->value.a); k++)
                    // {
                    //     SbsExpression *item = value->value.a[i];
                    //     
                    // }
                    fl_cstring_append(&interpolated_string, "(array)");
                    break;

                default:
                    fl_cstring_append(&interpolated_string, "(unk)");
                    break;
            }
            sbs_expression_free((SbsExpression*) value);
        }

        sbs_expression_free(var_expr);
    }

    if (format_index < format_length)
    {
        for (size_t i=format_index; i < format_length; i++)
            fl_cstring_append_char(&interpolated_string, string->format[i]);
    }

    return interpolated_string;
}

static void free_string_placeholder(SbsStringPlaceholder *placeholder)
{
    if (placeholder->variable)
        sbs_varinfo_free(placeholder->variable);

    fl_free(placeholder);
}

void sbs_string_free(SbsString *string)
{
    if (string->format)
        fl_cstring_free(string->format);

    if (string->args)
        fl_array_free_each_pointer(string->args, (FlArrayFreeElementFunc) free_string_placeholder);

    fl_free(string);
}

SbsString* sbs_string_copy(const SbsString *string)
{
    SbsString *copy = fl_malloc(sizeof(SbsString));
    copy->format = fl_cstring_dup(string->format);
    copy->is_constant = string->is_constant;

    if (!string->is_constant)
    {
        copy->args = fl_array_new(sizeof(SbsStringPlaceholder*), fl_array_length(string->args));

        for (size_t i=0; i < fl_array_length(string->args); i++)
        {
            copy->args[i] = fl_malloc(sizeof(SbsStringPlaceholder));
            copy->args[i]->index = string->args[i]->index;
            copy->args[i]->variable = sbs_varinfo_copy(string->args[i]->variable);
        }
    }

    return copy;
}
