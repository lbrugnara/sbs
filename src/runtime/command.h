#ifndef SBS_RUNTIME_COMMAND_H
#define SBS_RUNTIME_COMMAND_H

#include "../lang/command.h"
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
SbsCommand* sbs_command_resolve(const SbsValueCommand *value_command);

static inline SbsCommand** sbs_command_array_extend_from_value_command(SbsCommand **dest, SbsValueCommand **source)
{    
    if (!source)
        return dest;

    if (!dest)
        dest = fl_array_new(fl_array_element_size(source), 0);

    size_t dest_length = fl_array_length(dest);
    size_t elem_size = fl_array_element_size(source);
    size_t source_length = fl_array_length(source);

    dest = fl_array_resize(dest, dest_length + source_length);

    if (!dest)
        return NULL;

    for (size_t i=dest_length, j=0; i < dest_length + source_length; i++, j++)
    {
        dest[i] = sbs_command_resolve(source[j]);
    }

    return dest;
}

#endif /* SBS_RUNTIME_COMMAND_H */
