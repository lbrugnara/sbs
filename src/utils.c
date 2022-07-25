#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "utils.h"

FlArray* sbs_array_extend_copy_element(FlArray *dest, const FlArray *src, SbsArrayCopyElementFn copy_fn)
{
    if (!src) {
        return dest;
    }

    if (!dest) {
        dest = fl_array_new(fl_array_element_size(src), 0);
    }

    size_t dest_size = fl_array_length(dest);
    size_t elem_size = fl_array_element_size(src);
    size_t src_size = fl_array_length(src);

    dest = fl_array_resize(dest, dest_size + src_size);

    if (!dest) {
        return NULL;
    }

    for (size_t i=0; i < src_size; i++, dest_size++) {
        copy_fn((FlByte*) dest + dest_size * elem_size, (FlByte*) src + i * elem_size);
    }

    return dest;
}

FlArray* sbs_array_extend_copy_pointer(FlArray *dest, const FlArray *src, SbsArrayCopyPointerFn copy_fn)
{
    if (!src) {
        return dest;
    }

    if (!dest) {
        dest = fl_array_new(fl_array_element_size(src), 0);
    }

    size_t dest_size = fl_array_length(dest);
    size_t src_size = fl_array_length(src);

    dest = fl_array_resize(dest, dest_size + src_size);

    if (!dest) {
        return NULL;
    }

    for (size_t i=0; i < src_size; i++, dest_size++) {
        ((void **) dest)[dest_size] = copy_fn(((void **) src)[i]);
    }

    return dest;
}

void sbs_cstring_copy(char **dest, const char **src_str)
{
    if (!dest) {
        return;
    }

    if (!src_str || !*src_str) {
        memset(dest, 0, sizeof(char*));
        return;
    }

    char *copy = fl_cstring_dup(*src_str);

    memcpy(dest, &copy, sizeof(char*));
}

char* sbs_cstring_set(char *dest, const char *src)
{
    if (!src) {
        return dest;
    }

    if (!dest) {
        dest = fl_cstring_dup(src);
        return dest;
    }

    dest = fl_cstring_replace_realloc(dest, dest, src);
    return dest;
}

char** sbs_cstring_append_free(char **dest, char *src)
{
    dest = fl_cstring_append(dest, src);
    fl_cstring_free(src);
    return dest;
}
