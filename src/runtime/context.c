#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/containers/Hashtable.h>
#include "context.h"
#include "../result.h"
#include "../lang/file.h"

SbsContext* sbs_context_new(const SbsFile *file)
{
    SbsContext *context = fl_malloc(sizeof(SbsContext));

    context->file = file;
    context->host = sbs_host_get_info();
    context->evalctx = sbs_eval_context_new();

    context->resolvectx = sbs_resolve_context_new(context->file, context->host, context->evalctx);

    if (context->host->os == SBS_OS_UNK)
        goto error;

    if (context->host->arch == SBS_ARCH_UNK)
        goto error;

    // Init sbs variables
    fl_hashtable_add(context->evalctx->variables, "sbs.host.os", sbs_os_to_str(context->host->os));
    fl_hashtable_add(context->evalctx->variables, "sbs.host.arch", sbs_arch_to_str(context->host->arch));
    fl_hashtable_add(context->evalctx->variables, "sbs.win", "win");
    fl_hashtable_add(context->evalctx->variables, "sbs.linux", "linux");
    fl_hashtable_add(context->evalctx->variables, "sbs.x86", "x86");
    fl_hashtable_add(context->evalctx->variables, "sbs.x86_64", "x86_64");
    fl_hashtable_add(context->evalctx->variables, "sbs.armv7l", "armv7l");

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
    if (context->evalctx) sbs_eval_context_free(context->evalctx);
    if (context->resolvectx) sbs_resolve_context_free(context->resolvectx);
    if (context->executor) sbs_executor_free(context->executor);

    fl_free(context);
}

SbsContext* sbs_context_copy(const SbsContext *ctx)
{
    SbsContext *copy = sbs_context_new(ctx->file);

    // TODO: check if this is ok here
    copy->resolvectx->script_mode = ctx->resolvectx->script_mode;

    copy->preset = ctx->preset != NULL ? sbs_preset_resolve(copy->resolvectx, ctx->preset->name) : NULL;
    
    if (ctx->env != NULL)
        copy->env = sbs_env_resolve(copy->resolvectx, ctx->env->name, sbs_arch_to_str(ctx->env->arch));
    
    if (ctx->toolchain != NULL)
        copy->toolchain = sbs_toolchain_resolve(copy->resolvectx, ctx->toolchain->name);
    
    if (ctx->config != NULL)
        copy->config = sbs_config_resolve(copy->resolvectx, ctx->config->name);

    // We copy first the variables
    char **variables = fl_hashtable_keys(ctx->evalctx->variables);

    for (size_t i=0; i < fl_array_length(variables); i++)
        fl_hashtable_add(copy->evalctx->variables, variables[i], fl_hashtable_get(ctx->evalctx->variables, variables[i]));

    fl_array_free(variables);

    return copy;
}
