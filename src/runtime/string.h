#ifndef SBS_RUNTIME_STRING_H
#define SBS_RUNTIME_STRING_H

#include <stddef.h>
#include "variable.h"

struct SbsContext;

typedef struct SbsStringPlaceholder {
    SbsVariableInfo *variable;
    size_t index;
} SbsStringPlaceholder;

typedef struct SbsString {
    const char *format;
    SbsStringPlaceholder **args;
    bool is_constant;
} SbsString;

char* sbs_string_interpolate(struct SbsContext *context, SbsString *string);
void sbs_string_free(SbsString *string);

#endif /* SBS_RUNTIME_STRING_H */
