#ifndef SBS_UTILS_H
#define SBS_UTILS_H

#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/Slice.h>

typedef void(*SbsArrayCopyElementFn)(void*, const void*);
typedef void*(*SbsArrayCopyPointerFn)(const void*);

FlArray* sbs_array_extend_copy_element(FlArray *dest, const FlArray *src, SbsArrayCopyElementFn copy_fn);
FlArray* sbs_array_extend_copy_pointer(FlArray *dest, const FlArray *src, SbsArrayCopyPointerFn copy_fn);

void sbs_cstring_copy(char **dest, const char **src);
char* sbs_cstring_set(char *dest, const char *src);
char** sbs_cstring_append_free(char **dest, char *src);

static inline char** sbs_cstring_array_extend(char **dest, char * const * const src)
{
    return sbs_array_extend_copy_element(dest, src, (SbsArrayCopyElementFn) sbs_cstring_copy);
}

static inline char* sbs_slice_to_cstring(const struct FlSlice *slice)
{
    return fl_cstring_dup_n((const char*) slice->sequence, slice->length);
}

#endif /* SBS_UTILS_H */
