#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "string.h"

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

