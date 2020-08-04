#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"
#include "../utils.h"

SbsValueVariable* sbs_value_variable_new(const struct FlSlice *name, const struct FlSlice *namespace)
{
    SbsValueVariable *variable = fl_malloc(sizeof(SbsValueVariable));

    variable->name = sbs_slice_to_cstring(name);

    if (namespace != NULL)
    {
        variable->namespace = sbs_slice_to_cstring(namespace);
        variable->fqn = fl_cstring_vdup("%s.%s", variable->namespace, variable->name);
    }
    else
    {
        variable->fqn = fl_cstring_dup(variable->name);
    }

    return variable;
}

void sbs_value_variable_free(SbsValueVariable *variable)
{
    if (!variable)
        return;

    if (variable->name)
        fl_cstring_free(variable->name);

    if (variable->namespace)
        fl_cstring_free(variable->namespace);

    if (variable->fqn)
        fl_cstring_free(variable->fqn);

    fl_free(variable);
}

SbsValueVariable* sbs_value_variable_copy(const SbsValueVariable *variable)
{
    SbsValueVariable *copy = fl_malloc(sizeof(SbsValueVariable));

    copy->name = fl_cstring_dup(variable->name);
    copy->namespace = variable->namespace != NULL ? fl_cstring_dup(variable->namespace) : NULL;
    copy->fqn = fl_cstring_dup(variable->fqn);

    return copy;
}

SbsNodeVariableDefinition* sbs_node_variable_definition_new(void)
{
    SbsNodeVariableDefinition *var_def = fl_malloc(sizeof(SbsNodeVariableDefinition));

    return var_def;
}

void sbs_node_variable_definition_free(SbsNodeVariableDefinition *var_def)
{
    if (var_def->name)
        sbs_value_variable_free(var_def->name);

    switch (var_def->kind)
    {
        case SBS_VALUE_VAR_STR:
            fl_cstring_free(var_def->value.s);
            break;

        default: break;
    }

    fl_free(var_def);
}
