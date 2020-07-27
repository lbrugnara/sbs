#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "environment.h"
#include "variable.h"

void sbs_section_env_free(SbsSectionEnv *env)
{
    if (!env)
        return;

    if (env->os)
        sbs_value_variable_free(env->os);

    if (env->arch)
        fl_array_free_each_pointer(env->arch, (FlArrayFreeElementFunc) sbs_value_variable_free);

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each_pointer(env->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->type)
        fl_cstring_free(env->type);

    if (env->terminal)
        fl_cstring_free(env->terminal);

    if (env->args)
        fl_array_free_each_pointer(env->args, (FlArrayFreeElementFunc) fl_cstring_free);

    sbs_property_actions_free(&env->actions);

    free(env);
}

SbsSectionEnv* sbs_section_env_new(const struct FlSlice *name)
{
    SbsSectionEnv *env = fl_malloc(sizeof(SbsSectionEnv));

    env->name = sbs_slice_to_str(name);

    return env;
}
