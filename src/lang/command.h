#ifndef SBS_LANG_COMMAND_H
#define SBS_LANG_COMMAND_H

#include "../utils.h"

typedef struct SbsValueCommand {
    enum {
        SBS_COMMAND_STRING,
        SBS_COMMAND_NAME
    } type;
    const char *value;
} SbsValueCommand;

void sbs_value_command_free(SbsValueCommand *str);
void sbs_value_command_copy(SbsValueCommand *dest, const SbsValueCommand *src_obj);

inline SbsValueCommand* sbs_value_command_array_extend(SbsValueCommand *dest, SbsValueCommand const * const src)
{
    return sbs_array_extend(dest, src, (SbsArrayCopyElementFn) sbs_value_command_copy);
}

#endif /* SBS_LANG_COMMAND_H */
