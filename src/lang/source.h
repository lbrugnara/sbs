#ifndef SBS_LANG_SOURCE_H
#define SBS_LANG_SOURCE_H

#include "../utils.h"

typedef enum SbsSourceType {
    SBS_SOURCE_STRING,
    SBS_SOURCE_NAME
} SbsSourceType;

typedef struct SbsSource {
    SbsSourceType type;
    const char *value;
} SbsSource;

void sbs_source_free(SbsSource *str);
SbsSource* sbs_source_copy(const SbsSource *source);

static inline SbsSource** sbs_source_array_extend(SbsSource **dest, SbsSource **source)
{
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_source_copy);
}

#endif /* SBS_LANG_SOURCE_H */
