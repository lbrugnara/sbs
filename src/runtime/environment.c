#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "environment.h"

void sbs_env_free(SbsEnv *env)
{
    if (!env)
        return;

    if (env->host)
        sbs_host_free(env->host);

    if (env->arch)
        fl_array_free(env->arch);

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

    if (env->actions.before)
        fl_array_free_each(env->actions.before, (FlArrayFreeElementFunc) sbs_value_command_free);
    
    if (env->actions.after)
        fl_array_free_each(env->actions.after, (FlArrayFreeElementFunc) sbs_value_command_free);

    free(env);
}
