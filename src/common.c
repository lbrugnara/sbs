#include <fllib.h>
#include "common.h"
#include "parser/common.h"

void sbs_common_free_string(void *obj)
{
    if (!obj)
        return;

    char *str = *(char**)obj;
    fl_cstring_free(str);
}

void sbs_common_free_string_or_id(void *obj)
{
    if (!obj)
        return;

    struct SbsStringOrId *str = (struct SbsStringOrId*)obj;

    if (str->value)
        fl_cstring_free(str->value);
}

FlArray sbs_common_extend_array(FlArray dest, FlArray src)
{
    if (!src)
        return dest;

    if (!dest)
        dest = fl_array_new(fl_array_element_size(src), 0);

    return fl_array_combine(dest, src);
}

FlArray sbs_common_extend_array_copy_pointers(FlArray dest, FlArray src, void(*copy_func)(void*, const void*, size_t))
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
        copy_func((FlByte*)dest + dest_size * elem_size, (FlByte*)src + i * elem_size, elem_size);

    return dest;
}

void sbs_common_copy_string(void *dest, const void *src, size_t elem_size)
{
    if (!src || !dest)
        return;

    char **src_str = (char**)src;

    if (!src_str || !*src_str)
    {
        memset(dest, 0, elem_size);
        return;
    }

    char *copy = fl_cstring_dup(*src_str);

    memcpy(dest, &copy, elem_size);
}

void sbs_common_copy_string_or_id(void *dest, const void *src, size_t elem_size)
{
    if (!src || !dest)
        return;

    struct SbsStringOrId *src_obj = (struct SbsStringOrId*)src;

    if (!src_obj)
        return;

    struct SbsStringOrId copy = {
        .type = src_obj->type,
        .value = fl_cstring_dup(src_obj->value)
    };

    memcpy(dest, &copy, elem_size);
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
