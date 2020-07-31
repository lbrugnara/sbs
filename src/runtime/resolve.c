#include <fllib/Mem.h>
#include "resolve.h"

SbsResolveContext* sbs_resolve_context_new(const SbsFile *file, const SbsHostInfo *host, SbsEvalContext *evalctx)
{
    SbsResolveContext *context = fl_malloc(sizeof(SbsResolveContext));

    context->file = file;
    context->symbols = evalctx;
    context->host = host;

    return context;
}

void sbs_resolve_context_free(SbsResolveContext *context)
{
    fl_free(context);
}
