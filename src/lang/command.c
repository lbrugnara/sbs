#include <stdlib.h>
#include <fllib/Cstring.h>
#include "command.h"

void sbs_value_command_free(SbsValueCommand *str)
{
    if (str->value)
        fl_cstring_free(str->value);

    // SbsValueCommand are not malloc'ed
}

void sbs_value_command_copy(SbsValueCommand *dest, const SbsValueCommand *src_obj)
{
    if (!dest)
        return;

    if (!src_obj)
    {
        memset(dest, 0, sizeof(SbsValueCommand));
        return;
    }

    SbsValueCommand copy = {
        .type = src_obj->type,
        .value = fl_cstring_dup(src_obj->value)
    };

    memcpy(dest, &copy, sizeof(SbsValueCommand));
}
