#include "environment.h"
#include "../parser/environment.h"
#include "action.h"
#include "../host.h"

static SbsHostOs variable_to_host_os(SbsVariable *variable)
{
    if (variable == NULL)
        return SBS_OS_UNK;

    if (flm_cstring_equals(variable->name, "win"))
        return SBS_OS_WIN;

    if (flm_cstring_equals(variable->name, "linux"))
        return SBS_OS_LINUX;

    return SBS_OS_UNK;
}

static SbsHostArch* variable_array_to_host_arch(SbsVariable **variables)
{
    if (variables == NULL)
        return NULL;

    size_t vars_length = fl_array_length(variables);
    SbsHostArch *arch = fl_array_new(sizeof(SbsHostArch), vars_length);

    for (size_t i=0; i < vars_length; i++)
    {
        if (flm_cstring_equals("x86", variables[i]->name))
        {
            arch[i] = SBS_ARCH_X86;
        }
        else if (flm_cstring_equals("x86_64", variables[i]->name))
        {
            arch[i] = SBS_ARCH_X86_64;
        }
        else if (flm_cstring_equals("arm", variables[i]->name))
        {
            arch[i] = SBS_ARCH_ARM;
        }
        else if (flm_cstring_equals("arm64", variables[i]->name))
        {
            arch[i] = SBS_ARCH_ARM64;
        }
        else
        {
            arch[i] = SBS_ARCH_UNK;
        }
    }

    return arch;
}

SbsEnv* sbs_env_resolve(SbsContext *context, const char *env_name)
{
    SbsSectionEnv *env_section = fl_hashtable_get(context->file->envs, env_name);

    if (!env_section)
        return NULL;

    SbsEnv *env_object = fl_malloc(sizeof(SbsEnv));

    env_object->name = sbs_common_set_string(env_object->name, env_section->name);
    env_object->type = sbs_common_set_string(env_object->type, env_section->type);
    env_object->terminal = sbs_common_set_string(env_object->terminal, env_section->terminal);
    env_object->args = sbs_common_extend_array_copy(env_object->args, env_section->args, (SbsArrayCopyElementFn) sbs_common_copy_string);
    env_object->variables = sbs_common_extend_array_copy(env_object->variables, env_section->variables, (SbsArrayCopyElementFn) sbs_common_copy_string);
    env_object->os = variable_to_host_os(env_section->os);
    env_object->arch = variable_array_to_host_arch(env_section->arch);

    env_object->actions.before = sbs_common_extend_array_copy(env_object->actions.before, env_section->actions.before, (SbsArrayCopyElementFn) sbs_common_copy_string_or_id);
    env_object->actions.after = sbs_common_extend_array_copy(env_object->actions.after, env_section->actions.after, (SbsArrayCopyElementFn) sbs_common_copy_string_or_id);

    return env_object;
}

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
        fl_array_free_each(env->actions.before, (FlArrayFreeElementFunc) sbs_common_free_string_or_id);
    
    if (env->actions.after)
        fl_array_free_each(env->actions.after, (FlArrayFreeElementFunc) sbs_common_free_string_or_id);

    free(env);
}
