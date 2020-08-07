#ifndef SBS_LANG_COMMAND_H
#define SBS_LANG_COMMAND_H

#include "string.h"
#include "../utils.h"

typedef enum SbsCommandType {
    SBS_COMMAND_STRING,
    SBS_COMMAND_NAME
} SbsCommandType;

typedef struct SbsCommand {
    SbsCommandType type;
    SbsString *value;
} SbsCommand;

void sbs_command_free(SbsCommand *str);
SbsCommand* sbs_command_copy(const SbsCommand *source);

static inline SbsCommand** sbs_command_array_extend(SbsCommand **dest, SbsCommand **source)
{    
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_command_copy);
}

#endif /* SBS_LANG_COMMAND_H */
