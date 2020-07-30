#include "string.h"

SbsString* sbs_string_resolve(SbsValueString *value_string)
{
    SbsString *string = fl_malloc(sizeof(SbsString));
    string->format = fl_cstring_dup(value_string->format);

    size_t value_string_length = fl_array_length(value_string->args);

    if (value_string_length == 0)
    {
        string->is_constant = true;
        return string;
    }

    string->is_constant = false;
    string->args = fl_array_new(sizeof(SbsStringPlaceholder*), value_string_length);

    for (size_t i = 0; i < value_string_length; i++)
    {
        SbsValueStringVariablePlaceholder *value_ph = value_string->args[i];
        
        SbsStringPlaceholder *ph = fl_malloc(sizeof(SbsStringPlaceholder));
        ph->index = value_ph->index;
        ph->variable = sbs_varinfo_new(value_ph->variable->name, value_ph->variable->namespace);

        string->args[i] = ph;
    }

    return string;
}
