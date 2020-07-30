#ifndef SBS_RESOLVE_STRING_H
#define SBS_RESOLVE_STRING_H

#include "../string.h"
#include "../context.h"
#include "../../lang/string.h"

SbsString* sbs_string_resolve(SbsValueString *value_string);

static inline SbsString** sbs_cstring_array_extend_convert(SbsString **dest, SbsValueString **source)
{    
    if (!source)
        return dest;

    if (!dest)
        dest = fl_array_new(fl_array_element_size(source), 0);

    size_t dest_length = fl_array_length(dest);
    size_t elem_size = fl_array_element_size(source);
    size_t source_length = fl_array_length(source);

    dest = fl_array_resize(dest, dest_length + source_length);

    if (!dest)
        return NULL;

    for (size_t i=dest_length, j=0; i < dest_length + source_length; i++, j++)
    {
        dest[i] = sbs_string_resolve(source[j]);
    }

    return dest;
}

#endif /* SBS_RESOLVE_STRING_H */
