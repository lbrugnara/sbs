#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "environment.h"
#include "../host.h"
#include "../../lang/environment.h"

static SbsHostOs variable_to_host_os(SbsValueVariable *variable)
{
    if (variable == NULL)
        return SBS_OS_UNK;

    if (flm_cstring_equals(variable->name, "win"))
        return SBS_OS_WIN;

    if (flm_cstring_equals(variable->name, "linux"))
        return SBS_OS_LINUX;

    return SBS_OS_UNK;
}

static SbsHostArch* variable_array_to_host_arch(SbsValueVariable **variables)
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

    env_object->name = sbs_string_set(env_object->name, env_section->name);
    env_object->type = sbs_string_set(env_object->type, env_section->type);
    env_object->terminal = sbs_string_set(env_object->terminal, env_section->terminal);
    env_object->args = sbs_string_array_extend(env_object->args, env_section->args);
    env_object->variables = sbs_string_array_extend(env_object->variables, env_section->variables);
    env_object->os = variable_to_host_os(env_section->os);
    env_object->arch = variable_array_to_host_arch(env_section->arch);

    env_object->actions.before = sbs_value_command_array_extend(env_object->actions.before, env_section->actions.before);
    env_object->actions.after = sbs_value_command_array_extend(env_object->actions.after, env_section->actions.after);

    return env_object;
}
