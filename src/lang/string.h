#ifndef SBS_LANG_STRING_H
#define SBS_LANG_STRING_H

#include <stddef.h>
#include <fllib/Array.h>
#include "variable.h"
#include "../runtime/eval.h"
#include "../utils.h"

struct SbsEvalContext;

typedef struct SbsStringPlaceholder {
    SbsVariable *variable;
    size_t index;
} SbsStringPlaceholder;

typedef struct SbsString {
    const char *format;
    SbsStringPlaceholder **args;
    bool is_constant;
} SbsString;

SbsString* sbs_string_new(char *format, bool is_constant);
char* sbs_string_interpolate(struct SbsEvalContext *context, SbsString *string);
void sbs_string_free(SbsString *string);
SbsString* sbs_string_copy(const SbsString *string);

static inline SbsString* sbs_string_set(SbsString *dest, const SbsString *src)
{
    if (!src)
        return dest;

    if (!dest)
    {
        dest = sbs_string_copy(src);
        return dest;
    }

    sbs_string_free(dest);
    dest = sbs_string_copy(src);
    return dest;
}

static inline SbsString** sbs_string_array_extend(SbsString **dest, SbsString **source)
{    
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_string_copy);
}

static inline SbsString** sbs_string_array_extend_from_value_string(SbsString **dest, SbsString **source)
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
        dest[i] = sbs_string_copy(source[j]);
    }

    return dest;
}

#endif /* SBS_LANG_STRING_H */
