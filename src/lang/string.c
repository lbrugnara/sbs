#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "string.h"

SbsValueString* sbs_value_string_new(char *format)
{
    SbsValueString *string = fl_malloc(sizeof(SbsValueString));

    string->format = format;
    string->args = fl_array_new(sizeof(SbsValueStringVariablePlaceholder*), 0);

    return string;
}

static void free_value_string_placeholder(SbsValueStringVariablePlaceholder *placeholder)
{
    if (placeholder->variable)
        sbs_value_variable_free(placeholder->variable);

    fl_free(placeholder);
}

void sbs_value_string_free(SbsValueString *string)
{
    if (string->format)
        fl_cstring_free(string->format);

    if (string->args)
        fl_array_free_each_pointer(string->args, (FlArrayFreeElementFunc) free_value_string_placeholder);

    fl_free(string);
}

SbsValueString* sbs_value_string_copy(const SbsValueString *string)
{
    SbsValueString *copy = fl_malloc(sizeof(SbsValueString));

    copy->format = fl_cstring_dup(string->format);
    copy->args = fl_array_new(sizeof(SbsValueStringVariablePlaceholder*), fl_array_length(string->args));

    for (size_t i=0; i < fl_array_length(string->args); i++)
    {
        copy->args[i] = fl_malloc(sizeof(SbsValueStringVariablePlaceholder));
        copy->args[i]->index = string->args[i]->index;
        copy->args[i]->variable = sbs_value_variable_copy(string->args[i]->variable);
    }

    return copy;
}
