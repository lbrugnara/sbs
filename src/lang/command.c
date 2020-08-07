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
