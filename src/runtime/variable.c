#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "variable.h"

SbsVariableInfo* sbs_varinfo_new(const char *name, const char *namespace)
{
    SbsVariableInfo *varinfo = fl_malloc(sizeof(SbsVariableInfo));

    varinfo->name = fl_cstring_dup(name);

    if (namespace != NULL)
    {
        varinfo->namespace = fl_cstring_dup(namespace);
        varinfo->fqn = fl_cstring_vdup("%s.%s", varinfo->namespace, varinfo->name);
    }
    else
    {
        varinfo->fqn = fl_cstring_dup(varinfo->name);
    }

    return varinfo;
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
