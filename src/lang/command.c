#include <stdlib.h>
#include <fllib/Cstring.h>
#include "command.h"

void sbs_command_free(SbsCommand *command)
{
    if (command->type == SBS_COMMAND_STRING && command->value.str != NULL)
    {
        sbs_expression_free((SbsExpression*) command->value.str);
    }
    else if (command->type == SBS_COMMAND_NAME && command->value.id != NULL)
    {
        sbs_expression_free((SbsExpression*) command->value.id);
    }

    fl_free(command);
}

SbsCommand* sbs_command_copy(const SbsCommand *source)
{
    if (!source)
        return NULL;

    SbsCommand *copy = fl_malloc(sizeof(SbsCommand));

    copy->type = source->type;

    if (source->type == SBS_COMMAND_STRING)
    {
        copy->value.str = source->value.str != NULL ? (SbsStringExpr*) sbs_expression_copy((SbsExpression*) source->value.str) : NULL;
    }
    else if (source->type == SBS_COMMAND_NAME)
    {
        copy->value.id = source->value.id != NULL ? (SbsIdentifierExpr*) sbs_expression_copy((SbsExpression*) source->value.id) : NULL;
    }

    return copy;
}
