#ifndef SBS_LANG_SOURCE_H
#define SBS_LANG_SOURCE_H

#include "../utils.h"

typedef enum SbsValueSourceType {
    SBS_VALUE_SOURCE_STRING,
    SBS_VALUE_SOURCE_NAME
} SbsValueSourceType;

typedef struct SbsValueSource {
    SbsValueSourceType type;
    const char *value;
} SbsValueSource;

void sbs_value_source_free(SbsValueSource *str);

#endif /* SBS_LANG_SOURCE_H */
