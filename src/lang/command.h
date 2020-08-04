#ifndef SBS_LANG_COMMAND_H
#define SBS_LANG_COMMAND_H

#include "string.h"
#include "../utils.h"

typedef enum SbsValueCommandType {
    SBS_VALUE_COMMAND_STRING,
    SBS_VALUE_COMMAND_NAME
} SbsValueCommandType;

typedef struct SbsValueCommand {
    SbsValueCommandType type;
    SbsValueString *value;
} SbsValueCommand;

void sbs_value_command_free(SbsValueCommand *str);
SbsValueCommand* sbs_value_command_copy(const SbsValueCommand *src_obj);

static inline SbsValueCommand** sbs_value_command_array_extend(SbsValueCommand **dest, SbsValueCommand **src)
{
    return sbs_array_extend_copy_pointer(dest, src, (SbsArrayCopyPointerFn) sbs_value_command_copy);
}

#endif /* SBS_LANG_COMMAND_H */
