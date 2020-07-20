#ifndef SBS_COMMON_H
#define SBS_COMMON_H

#include <fllib.h>
#include "../parser/helpers.h"

#define SBS_BASE_OBJECT_KEY "#base"

typedef void(*SbsArrayCopyElementFn)(void*, const void*);

void sbs_common_free_string_or_id(void *obj);
void sbs_common_free_variable(SbsVariable *variable);

char** sbs_common_append_string_free(char **dest, char *src);

FlArray* sbs_common_extend_array(FlArray *dest, FlArray *src);
FlArray* sbs_common_extend_array_copy(FlArray *dest, FlArray *src, SbsArrayCopyElementFn copy_fn);

void sbs_common_copy_string(char **dest, const char **src);
void sbs_common_copy_string_or_id(SbsStringOrId *dest, const SbsStringOrId *src);

char* sbs_common_set_string(char *dest, const char *src);

#endif /* SBS_COMMON_H */
