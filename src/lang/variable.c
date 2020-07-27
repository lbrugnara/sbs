#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"
#include "../utils.h"

SbsValueVariable* sbs_value_variable_new(const struct FlSlice *name, const struct FlSlice *namespace)
{
    SbsValueVariable *variable = fl_malloc(sizeof(SbsValueVariable));

    variable->name = sbs_slice_to_str(name);
    variable->namespace = sbs_slice_to_str(namespace);

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

    fl_free(variable);
}
