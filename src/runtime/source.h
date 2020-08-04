#ifndef SBS_RUNTIME_SOURCE_H
#define SBS_RUNTIME_SOURCE_H

#include "../lang/source.h"
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
SbsSource* sbs_source_resolve(const SbsValueSource *value_source);

static inline SbsSource** sbs_source_array_extend(SbsSource **dest, SbsSource **source)
{
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_source_copy);
}

static inline SbsSource** sbs_source_array_extend_from_value_source(SbsSource **dest, SbsValueSource **source)
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
        dest[i] = sbs_source_resolve(source[j]);
    }

    return dest;
}

#endif /* SBS_RUNTIME_SOURCE_H */
