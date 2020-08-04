#include <stdlib.h>
#include <fllib/Cstring.h>
#include "command.h"

void sbs_value_command_free(SbsValueCommand *str)
{
    if (str->value)
        sbs_value_string_free(str->value);

    fl_free(str);
}

SbsValueCommand* sbs_value_command_copy(const SbsValueCommand *src_obj)
{
    if (!src_obj)
        return NULL;

    SbsValueCommand *copy = fl_malloc(sizeof(SbsValueCommand));

    copy->type = src_obj->type;
    copy->value = sbs_value_string_copy(src_obj->value);

    return copy;
}
