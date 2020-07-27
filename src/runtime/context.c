#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/containers/Hashtable.h>
#include "context.h"
#include "executor.h"
#include "resolvers/environment.h"
#include "resolvers/configuration.h"
#include "resolvers/preset.h"
#include "resolvers/target.h"
#include "resolvers/toolchain.h"
#include "../result.h"
#include "../lang/file.h"

SbsContext* sbs_context_new(const SbsFile *file)
{
    SbsContext *context = fl_malloc(sizeof(SbsContext));

    context->file = file;
    context->symbols = sbs_eval_context_new();

    context->host = sbs_host_get_info();

    if (context->host->os == SBS_OS_UNK)
        goto error;

    if (context->host->arch == SBS_ARCH_UNK)
        goto error;

    fl_hashtable_add(context->symbols->variables, "sbs.os", sbs_host_os_to_str(context->host->os));
    fl_hashtable_add(context->symbols->variables, "sbs.arch", sbs_host_arch_to_str(context->host->arch));

    return context;

error:
    sbs_context_free(context);

    return NULL;
}

void sbs_context_free(SbsContext *context)
{
    if (context->config) sbs_config_free(context->config);
    if (context->targets) fl_array_free_each_pointer(context->targets, (FlArrayFreeElementFunc) sbs_target_free);
    if (context->toolchain) sbs_toolchain_free(context->toolchain);
    if (context->env) sbs_env_free(context->env);
    if (context->preset) sbs_preset_free(context->preset);
    if (context->host) sbs_host_free(context->host);
    if (context->symbols) sbs_eval_context_free(context->symbols);
    if (context->executor) sbs_executor_free(context->executor);

    fl_free(context);
}

SbsContext* sbs_context_copy(const SbsContext *ctx)
{
    SbsContext *copy = sbs_context_new(ctx->file);    

    copy->preset = ctx->preset != NULL ? sbs_preset_resolve(copy, ctx->preset->name) : NULL;
    
    if (ctx->env != NULL)
    {
        copy->env = sbs_env_resolve(copy, ctx->env->name);
        fl_hashtable_add(copy->symbols->variables, "sbs.env", copy->env->name);
    }
    
    if (ctx->toolchain != NULL)
    {
        copy->toolchain = sbs_toolchain_resolve(copy, ctx->toolchain->name);
        fl_hashtable_add(copy->symbols->variables, "sbs.toolchain", copy->toolchain->name);
    }
    
    if (ctx->config != NULL)
    {
        copy->config = sbs_config_resolve(copy, ctx->config->name);
        fl_hashtable_add(copy->symbols->variables, "sbs.config", copy->config->name);
    }

    // We copy first the variables
    char **variables = fl_hashtable_keys(ctx->symbols->variables);

    for (size_t i=0; i < fl_array_length(variables); i++)
        fl_hashtable_add(copy->symbols->variables, variables[i], fl_hashtable_get(ctx->symbols->variables, variables[i]));

    fl_array_free(variables);

    return copy;
}
