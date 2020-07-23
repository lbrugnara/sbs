#include <stdlib.h>
#include <fllib/Cstring.h>
#include "types.h"

void sbs_string_or_id_free(SbsStringOrId *str)
{
    if (str->value)
        fl_cstring_free(str->value);

    // SbsStringOrId are not malloc'ed
}

void sbs_string_or_id_copy(SbsStringOrId *dest, const SbsStringOrId *src_obj)
{
    if (!dest)
        return;

    if (!src_obj)
    {
        memset(dest, 0, sizeof(SbsStringOrId));
        return;
    }

    SbsStringOrId copy = {
        .type = src_obj->type,
        .value = fl_cstring_dup(src_obj->value)
    };

    memcpy(dest, &copy, sizeof(SbsStringOrId));
}

void sbs_variable_free(SbsVariable *variable)
{
    if (!variable)
        return;

    if (variable->name)
        fl_cstring_free(variable->name);

    if (variable->namespace)
        fl_cstring_free(variable->namespace);

    fl_free(variable);
}
