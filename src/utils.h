#ifndef SBS_UTILS_H
#define SBS_UTILS_H

#include <fllib/Array.h>

typedef void(*SbsArrayCopyElementFn)(void*, const void*);

FlArray* sbs_array_extend(FlArray *dest, const FlArray *src, SbsArrayCopyElementFn copy_fn);

void sbs_string_copy(char **dest, const char **src);
char* sbs_string_set(char *dest, const char *src);
char** sbs_string_append_free(char **dest, char *src);

inline char** sbs_string_array_extend(char **dest, char * const * const src)
{
    return sbs_array_extend(dest, src, (SbsArrayCopyElementFn) sbs_string_copy);
}

#endif /* SBS_UTILS_H */
