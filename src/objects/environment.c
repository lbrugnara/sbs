#include "environment.h"
#include "../parser/environment.h"
#include "action.h"

struct SbsEnv* sbs_env_resolve(const struct SbsFile *file, const char *env_name)
{
    struct SbsEnvSection *env_section = fl_hashtable_get(file->envs, env_name);

    if (!env_section)
        return NULL;

    struct SbsEnv *env_object = fl_malloc(sizeof(struct SbsEnv));

    env_object->name = sbs_common_set_string(env_object->name, env_section->name);
    env_object->type = sbs_common_set_string(env_object->type, env_section->type);
    env_object->terminal = sbs_common_set_string(env_object->terminal, env_section->terminal);
    env_object->args = sbs_common_extend_array_copy_pointers(env_object->args, env_section->args, sbs_common_copy_string);
    env_object->variables = sbs_common_extend_array_copy_pointers(env_object->variables, env_section->variables, sbs_common_copy_string);
    
    struct SbsAction **before_actions = sbs_action_resolve_all(file, env_section->actions.before, env_name);
    if (before_actions)
    {
        env_object->actions.before = sbs_common_extend_array(env_object->actions.before, before_actions);
        fl_array_free(before_actions);
    }

    struct SbsAction **after_actions = sbs_action_resolve_all(file, env_section->actions.after, env_name);
    if (after_actions)
    {
        env_object->actions.after = sbs_common_extend_array(env_object->actions.after, after_actions);
        fl_array_free(after_actions);
    }

    return env_object;
}

void sbs_env_free(struct SbsEnv *env)
{
    if (!env)
        return;

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each(env->variables, sbs_common_free_string);

    if (env->type)
        fl_cstring_free(env->type);

    if (env->terminal)
        fl_cstring_free(env->terminal);

    if (env->args)
        fl_array_free_each(env->args, sbs_common_free_string);

    if (env->actions.before)
        sbs_action_free_all(env->actions.before);
    
    if (env->actions.after)
        sbs_action_free_all(env->actions.after);

    free(env);
}
