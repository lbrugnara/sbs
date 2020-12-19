#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "environment.h"
#include "host.h"
#include "../lang/environment.h"

SbsEnv* sbs_env_new(const char *name)
{
    SbsEnv *env = fl_malloc(sizeof(SbsEnv));
    env->name = fl_cstring_dup(name);
    return env;
}

void sbs_env_free(SbsEnv *env)
{
    if (!env)
        return;

    if (env->host)
        sbs_host_free(env->host);

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each_pointer(env->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->shell_command)
        fl_cstring_free(env->shell_command);

    if (env->shell_args)
        fl_array_free_each_pointer(env->shell_args, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->actions.before)
        fl_array_free_each_pointer(env->actions.before, (FlArrayFreeElementFunc) sbs_command_free);
    
    if (env->actions.after)
        fl_array_free_each_pointer(env->actions.after, (FlArrayFreeElementFunc) sbs_command_free);

    free(env);
}


static SbsOs eval_os_expression(SbsEvalContext *evalctx, SbsExpression *os_expression)
{
    if (os_expression == NULL)
        return SBS_OS_UNK;

    SbsValueExpr *os_value = sbs_eval_expr(evalctx, os_expression);

    if (os_value == NULL)
        return SBS_OS_UNK;

    SbsOs os = SBS_OS_UNK;

    if (flm_cstring_equals("win", os_value->value.s))
    {
        os = SBS_OS_WIN;
    }
    else if (flm_cstring_equals("linux", os_value->value.s))
    {
        os = SBS_OS_LINUX;
    }

    sbs_expr_free((SbsExpression*) os_value);

    return os;
}

static SbsArch eval_arch_expression(SbsEvalContext *evalctx, SbsExpression *arch_expression)
{
    if (arch_expression == NULL)
        return SBS_ARCH_UNK;

    SbsValueExpr *arch_value = sbs_eval_expr(evalctx, arch_expression);

    if (arch_value == NULL)
        return SBS_ARCH_UNK;

    SbsArch arch = SBS_ARCH_UNK;

    if (flm_cstring_equals("x86", arch_value->value.s))
    {
        arch = SBS_ARCH_X86;
    }
    else if (flm_cstring_equals("x86_64", arch_value->value.s))
    {
        arch = SBS_ARCH_X86_64;
    }
    else if (flm_cstring_equals("arm", arch_value->value.s))
    {
        arch = SBS_ARCH_ARM;
    }
    else if (flm_cstring_equals("arm64", arch_value->value.s))
    {
        arch = SBS_ARCH_ARM64;
    }

    sbs_expr_free((SbsExpression*) arch_value);

    return arch;
}

SbsShellType parse_shell_type(const char *shell_type)
{
    if (shell_type == NULL)
        return SBS_SHELL_SYSTEM;

    if (flm_cstring_equals(shell_type, "bash"))
        return SBS_SHELL_BASH;

    if (flm_cstring_equals(shell_type, "cmd"))
        return SBS_SHELL_CMD;
    
    if (flm_cstring_equals(shell_type, "powershell"))
        return SBS_SHELL_POWERSHELL;

    return SBS_SHELL_UNK;
}

SbsEnv* sbs_env_resolve(SbsResolveContext *context, const char *env_name, const char *arch)
{
    SbsSectionEnv *env_section = fl_hashtable_get(context->file->envs, env_name);

    if (!env_section)
        return NULL;

    if (!context->script_mode && env_section->condition && !sbs_eval_expr_to_bool(context->evalctx, env_section->condition))
        return NULL;

    SbsEnv *env_object = sbs_env_new(env_section->name);
    
    env_object->shell_type = parse_shell_type(env_section->shell_type);
    env_object->shell_command = sbs_cstring_set(env_object->shell_command, env_section->shell_command);
    env_object->shell_args = sbs_cstring_array_extend(env_object->shell_args, env_section->shell_args);
    env_object->variables = sbs_cstring_array_extend(env_object->variables, env_section->variables);
    env_object->os = env_section->os != NULL ? eval_os_expression(context->evalctx, env_section->os) : context->host->os;

    env_object->arch = arch != NULL ? sbs_arch_from_str(arch) 
                                    : env_section->arch != NULL ? eval_arch_expression(context->evalctx, env_section->arch) 
                                                                : context->host->arch;

    env_object->actions.before = sbs_command_array_extend(env_object->actions.before, env_section->actions.before);
    env_object->actions.after = sbs_command_array_extend(env_object->actions.after, env_section->actions.after);

    env_object->host = sbs_host_new(env_object->os, env_object->arch);

    sbs_eval_context_add_variable(context->evalctx, "sbs.env", env_object->name);
    sbs_eval_context_add_variable(context->evalctx, "sbs.os", sbs_os_to_str(env_object->os));
    sbs_eval_context_add_variable(context->evalctx, "sbs.arch", sbs_arch_to_str(env_object->arch));

    char *shell_type = NULL;
    switch (env_object->shell_type)
    {
        case SBS_SHELL_BASH:
            shell_type = "bash";
            break;
        case SBS_SHELL_CMD:
            shell_type = "cmd";
            break;
        case SBS_SHELL_POWERSHELL:
            shell_type = "powershell";
            break;
        case SBS_SHELL_SYSTEM:
            shell_type = "system";
            break;

        default: break;
    }

    if (shell_type != NULL)
        sbs_eval_context_add_variable(context->evalctx, "sbs.shell", shell_type);

    return env_object;
}
