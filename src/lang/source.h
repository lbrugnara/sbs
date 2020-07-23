#ifndef SBS_LANG_SOURCE_H
#define SBS_LANG_SOURCE_H

#include "../utils.h"

typedef struct SbsValueSource {
    enum {
        SBS_SOURCE_STRING,
        SBS_SOURCE_NAME
    } type;
    const char *value;
} SbsValueSource;

void sbs_value_source_free(SbsValueSource *str);
void sbs_value_source_copy(SbsValueSource *dest, const SbsValueSource *src_obj);

inline SbsValueSource* sbs_value_source_array_extend(SbsValueSource *dest, SbsValueSource const * const src)
{
    return sbs_array_extend(dest, src, (SbsArrayCopyElementFn) sbs_value_source_copy);
}

#endif /* SBS_LANG_SOURCE_H */
