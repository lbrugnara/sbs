#ifndef SBS_LANG_STRING_H
#define SBS_LANG_STRING_H

#include <stddef.h>
#include <fllib/Array.h>
#include "parser.h"
#include "expr-variable.h"
#include "../utils.h"

struct SbsEvalContext;

typedef struct SbsStringPlaceholder {
    SbsVariableExpr *variable;
    size_t index;
} SbsStringPlaceholder;

typedef struct SbsString {
    const char *format;
    SbsStringPlaceholder **args;
    bool is_constant;
} SbsString;

SbsString* sbs_string_parse(SbsParser *parser);
SbsString** sbs_string_array_parse(SbsParser *parser);
SbsString* sbs_string_new(char *format, bool is_constant);
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

#endif /* SBS_LANG_STRING_H */
