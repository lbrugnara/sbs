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
        sbs_variable_free(env->os);

    if (env->arch)
        sbs_expression_free(env->arch);

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each_pointer(env->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->shell_type)
        fl_cstring_free(env->shell_type);

    if (env->shell_command)
        fl_cstring_free(env->shell_command);

    if (env->shell_args)
        fl_array_free_each_pointer(env->shell_args, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->condition)
        sbs_expression_free(env->condition);

    sbs_property_actions_free(&env->actions);

    free(env);
}

SbsSectionEnv* sbs_section_env_new(const struct FlSlice *name)
{
    SbsSectionEnv *env = fl_malloc(sizeof(SbsSectionEnv));

    env->name = sbs_slice_to_cstring(name);

    return env;
}
