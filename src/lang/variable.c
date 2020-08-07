#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"
#include "../utils.h"

SbsNodeVariableDefinition* sbs_node_variable_definition_new(void)
{
    SbsNodeVariableDefinition *var_def = fl_malloc(sizeof(SbsNodeVariableDefinition));

    return var_def;
}

void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def)
{
    if (var_def->name)
        sbs_expression_free((SbsExpression*) var_def->name);

    switch (var_def->kind)
    {
        case SBS_VARIABLE_TYPE_STRING:
            fl_cstring_free(var_def->value.s);
            break;

        default: break;
    }

    fl_free(var_def);
}
