#include <stdlib.h>
#include <fllib/Cstring.h>
#include "source.h"

void sbs_source_free(SbsSource *source)
{
    if (source->value)
        fl_cstring_free(source->value);

    fl_free(source);
}

SbsSource* sbs_source_copy(const SbsSource *source)
{
    if (!source)
        return NULL;

    SbsSource *copy = fl_malloc(sizeof(SbsSource));

    copy->type = source->type;
    copy->value = fl_cstring_dup(source->value);

    return copy;
}

SbsSource* sbs_source_resolve(const SbsValueSource *value_source)
{
    SbsSource *copy = fl_malloc(sizeof(SbsSource));

    copy->type = value_source->type == SBS_VALUE_SOURCE_NAME ? SBS_SOURCE_NAME : SBS_SOURCE_STRING;
    copy->value = fl_cstring_dup(value_source->value);

    return copy;
}
