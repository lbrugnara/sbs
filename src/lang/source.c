#include <stdlib.h>
#include <fllib/Cstring.h>
#include "source.h"

void sbs_value_source_free(SbsValueSource *str)
{
    if (str->value)
        fl_cstring_free(str->value);

    // SbsValueSource are not malloc'ed
}

void sbs_value_source_copy(SbsValueSource *dest, const SbsValueSource *src_obj)
{
    if (!dest)
        return;

    if (!src_obj)
    {
        memset(dest, 0, sizeof(SbsValueSource));
        return;
    }

    SbsValueSource copy = {
        .type = src_obj->type,
        .value = fl_cstring_dup(src_obj->value)
    };

    memcpy(dest, &copy, sizeof(SbsValueSource));
}
