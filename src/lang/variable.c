#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"
#include "../utils.h"

SbsVariableInfo* sbs_varinfo_new_from_slice(const struct FlSlice *name, const struct FlSlice *namespace)
{
    SbsVariableInfo *variable = fl_malloc(sizeof(SbsVariableInfo));

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

SbsVariableInfo* sbs_varinfo_new(const char *name, const char *namespace)
{
    return sbs_varinfo_new_from_slice(
            &flm_slice_new((const FlByte * const) name, 1, 0, strlen(name)), 
            (namespace != NULL ? &flm_slice_new((const FlByte * const) namespace, 1, 0, strlen(namespace)) : NULL));
}

void sbs_varinfo_free(SbsVariableInfo *varinfo)
{
    if (!varinfo)
        return;

    if (varinfo->name)
        fl_cstring_free(varinfo->name);

    if (varinfo->namespace)
        fl_cstring_free(varinfo->namespace);

    if (varinfo->fqn)
        fl_cstring_free(varinfo->fqn);

    fl_free(varinfo);
}

SbsVariableInfo* sbs_varinfo_copy(const SbsVariableInfo *varinfo)
{
    SbsVariableInfo *copy = fl_malloc(sizeof(SbsVariableInfo));

    copy->name = fl_cstring_dup(varinfo->name);
    copy->namespace = varinfo->namespace != NULL ? fl_cstring_dup(varinfo->namespace) : NULL;
    copy->fqn = fl_cstring_dup(varinfo->fqn);

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
        sbs_varinfo_free(var_def->name);

    switch (var_def->kind)
    {
        case SBS_VALUE_VAR_STR:
            fl_cstring_free(var_def->value.s);
            break;

        default: break;
    }

    fl_free(var_def);
}
