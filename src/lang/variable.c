#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"

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
