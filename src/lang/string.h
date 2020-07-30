#ifndef SBS_LANG_STRING_H
#define SBS_LANG_STRING_H

#include <stddef.h>
#include "variable.h"

typedef struct SbsValueStringVariablePlaceholder {
    SbsValueVariable *variable;
    size_t index;
} SbsValueStringVariablePlaceholder;

typedef struct SbsValueString {
    const char *format;
    SbsValueStringVariablePlaceholder **args;
} SbsValueString;

void sbs_value_string_free(SbsValueString *string);

#endif /* SBS_LANG_STRING_H */
