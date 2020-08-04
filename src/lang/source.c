#include <stdlib.h>
#include <fllib/Cstring.h>
#include "source.h"

void sbs_value_source_free(SbsValueSource *value_source)
{
    if (value_source->value)
        fl_cstring_free(value_source->value);

    fl_free(value_source);
}
