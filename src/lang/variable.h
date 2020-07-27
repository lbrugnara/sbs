#ifndef SBS_LANG_VARIABLE_H
#define SBS_LANG_VARIABLE_H

#include <fllib/Slice.h>

typedef struct SbsValueVariable {
    const char *namespace;
    const char *name;
} SbsValueVariable;

SbsValueVariable* sbs_value_variable_new(const struct FlSlice *name, const struct FlSlice *namespace);
void sbs_value_variable_free(SbsValueVariable *variable);

#endif /* SBS_LANG_VARIABLE_H */
