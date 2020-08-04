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

SbsValueString* sbs_value_string_new(char *format);
void sbs_value_string_free(SbsValueString *string);
SbsValueString* sbs_value_string_copy(const SbsValueString *string);

#endif /* SBS_LANG_STRING_H */
