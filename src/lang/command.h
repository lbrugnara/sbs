#ifndef SBS_LANG_COMMAND_H
#define SBS_LANG_COMMAND_H

#include "../utils.h"

typedef enum SbsValueCommandType {
    SBS_COMMAND_STRING,
    SBS_COMMAND_NAME
} SbsValueCommandType;

typedef struct SbsValueCommand {
    SbsValueCommandType type;
    const char *value;
} SbsValueCommand;

void sbs_value_command_free(SbsValueCommand *str);
void sbs_value_command_copy(SbsValueCommand *dest, const SbsValueCommand *src_obj);

static inline SbsValueCommand* sbs_value_command_array_extend(SbsValueCommand *dest, SbsValueCommand const * const src)
{
    return sbs_array_extend(dest, src, (SbsArrayCopyElementFn) sbs_value_command_copy);
}

#endif /* SBS_LANG_COMMAND_H */
