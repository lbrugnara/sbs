#include <stdlib.h>
#include <fllib/Cstring.h>
#include "command.h"

void sbs_command_free(SbsCommand *command)
{
    if (command->value)
        sbs_string_free(command->value);

    fl_free(command);
}

SbsCommand* sbs_command_copy(const SbsCommand *source)
{
    if (!source)
        return NULL;

    SbsCommand *copy = fl_malloc(sizeof(SbsCommand));

    copy->type = source->type;
    copy->value = sbs_string_copy(source->value);

    return copy;
}

SbsCommand* sbs_command_resolve(const SbsValueCommand *value_command)
{
    SbsCommand *copy = fl_malloc(sizeof(SbsCommand));

    copy->type = value_command->type == SBS_VALUE_COMMAND_NAME ? SBS_COMMAND_NAME : SBS_COMMAND_STRING;
    copy->value = sbs_string_copy(value_command->value);

    return copy;
}
