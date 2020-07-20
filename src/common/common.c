#include <fllib.h>
#include "common.h"
#include "../objects/action.h"
#include "../parser/helpers.h"

void sbs_common_free_string_or_id(void *obj)
{
    if (!obj)
        return;

    SbsStringOrId *str = (SbsStringOrId*)obj;

    if (str->value)
        fl_cstring_free(str->value);
}

void sbs_common_free_variable(SbsVariable *variable)
{
    if (!variable)
        return;

    if (variable->name)
        fl_cstring_free(variable->name);

    if (variable->namespace)
        fl_cstring_free(variable->namespace);

    fl_free(variable);
}

char** sbs_common_append_string_free(char **dest, char *src)
{
    dest = fl_cstring_append(dest, src);
    fl_cstring_free(src);
    return dest;
}

FlArray* sbs_common_extend_array(FlArray *dest, FlArray *src)
{
    if (!src)
        return dest;

    if (!dest)
        dest = fl_array_new(fl_array_element_size(src), 0);

    return fl_array_combine(dest, src);
}

FlArray* sbs_common_extend_array_copy(FlArray *dest, FlArray *src, SbsArrayCopyElementFn copy_fn)
{
    if (!src)
        return dest;

    if (!dest)
        dest = fl_array_new(fl_array_element_size(src), 0);

    size_t dest_size = fl_array_length(dest);
    size_t elem_size = fl_array_element_size(src);
    size_t src_size = fl_array_length(src);

    dest = fl_array_resize(dest, dest_size + src_size);

    if (!dest)
        return NULL;    

    for (size_t i=0; i < src_size; i++, dest_size++)
    {
        copy_fn((FlByte*) dest + dest_size * elem_size, (FlByte*)src + i * elem_size);
    }

    return dest;
}

void sbs_common_copy_string(char **dest, const char **src_str)
{
    if (!dest)
        return;

    if (!src_str || !*src_str)
    {
        memset(dest, 0, sizeof(char*));
        return;
    }

    char *copy = fl_cstring_dup(*src_str);

    memcpy(dest, &copy, sizeof(char*));
}

void sbs_common_copy_string_or_id(SbsStringOrId *dest, const SbsStringOrId *src_obj)
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

char* sbs_common_set_string(char *dest, const char *src)
{
    if (!src)
        return dest;

    if (!dest)
    {
        dest = fl_cstring_dup(src);
        return dest;
    }

    dest = fl_cstring_replace_realloc(dest, dest, src);
    return dest;
}
