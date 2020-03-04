#ifndef SBS_COMMON_H
#define SBS_COMMON_H

#include <fllib.h>
#include "parser/common.h"

#define SBS_BASE_OBJECT_KEY "#base"

void sbs_common_free_string(void *obj);
void sbs_common_free_string_or_id(void *obj);

FlArray* sbs_common_extend_array(FlArray *dest, FlArray *src);
FlArray* sbs_common_extend_array_copy_pointers(FlArray *dest, FlArray *src, void(*copy_func)(void*, const void*, size_t));
void sbs_common_copy_string(void *dest, const void *src, size_t elem_size);
void sbs_common_copy_string_or_id(void *dest, const void *src, size_t elem_size);
char* sbs_common_set_string(char *dest, const char *src);

#endif /* SBS_COMMON_H */
