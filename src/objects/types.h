#ifndef SBS_TYPES_H
#define SBS_TYPES_H

#include "../utils.h"

typedef struct SbsStringOrId {
    enum {
        SBS_STRING,
        SBS_IDENTIFIER
    } type;
    const char *value;
} SbsStringOrId;

typedef struct SbsVariable {
    const char *namespace;
    const char *name;
} SbsVariable;

void sbs_string_or_id_free(SbsStringOrId *str);
void sbs_string_or_id_copy(SbsStringOrId *dest, const SbsStringOrId *src_obj);

void sbs_variable_free(SbsVariable *variable);

inline SbsStringOrId* sbs_string_or_id_array_extend(SbsStringOrId *dest, SbsStringOrId const * const src)
{
    return sbs_array_extend(dest, src, (SbsArrayCopyElementFn) sbs_string_or_id_copy);
}

#endif /* SBS_TYPES_H */
