#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "string.h"
#include "variable.h"

SbsString* sbs_string_new(char *format, bool is_constant)
{
    SbsString *string = fl_malloc(sizeof(SbsString));

    string->format = format;
    string->is_constant = is_constant;

    if (!is_constant)
        string->args = fl_array_new(sizeof(SbsStringPlaceholder*), 0);

    return string;
}

static void free_string_placeholder(SbsStringPlaceholder *placeholder)
{
    if (placeholder->variable)
        sbs_variable_free(placeholder->variable);

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
            copy->args[i]->variable = sbs_variable_copy(string->args[i]->variable);
        }
    }

    return copy;
}
