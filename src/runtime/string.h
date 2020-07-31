#ifndef SBS_RUNTIME_STRING_H
#define SBS_RUNTIME_STRING_H

#include <stddef.h>
#include <fllib/Array.h>
#include "variable.h"
#include "eval.h"
#include "../lang/string.h"

typedef struct SbsStringPlaceholder {
    SbsVariableInfo *variable;
    size_t index;
} SbsStringPlaceholder;

typedef struct SbsString {
    const char *format;
    SbsStringPlaceholder **args;
    bool is_constant;
} SbsString;

char* sbs_string_interpolate(SbsEvalContext *context, SbsString *string);
void sbs_string_free(SbsString *string);
SbsString* sbs_string_resolve(const SbsValueString *value_string);

static inline SbsString* sbs_string_set(SbsString *dest, const SbsValueString *src)
{
    if (!src)
        return dest;

    if (!dest)
    {
        dest = sbs_string_resolve(src);
        return dest;
    }

    sbs_string_free(dest);
    dest = sbs_string_resolve(src);
    return dest;
}

static inline SbsString** sbs_string_array_extend_convert(SbsString **dest, SbsValueString **source)
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


#endif /* SBS_RUNTIME_STRING_H */
