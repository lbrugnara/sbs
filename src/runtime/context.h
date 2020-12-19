#ifndef SBS_RUNTIME_CONTEXT_H
#define SBS_RUNTIME_CONTEXT_H

#include <stdbool.h>
#include "../result.h"
#include "../lang/file.h"
#include "configuration.h"
#include "eval.h"
#include "executor.h"
#include "environment.h"
#include "host.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"
#include "resolve.h"

typedef struct SbsContext {
    const SbsFile *file;
    SbsHostInfo *host;
    SbsResolveContext *resolvectx;
    SbsEvalContext *evalctx;
    SbsExecutor *executor;
    SbsEnv *env;
    SbsToolchain *toolchain;
    SbsTarget **targets;
    SbsConfiguration *config;
    SbsPreset *preset;
} SbsContext;

SbsContext* sbs_context_new(const SbsFile *file);
void sbs_context_free(SbsContext *context);
SbsContext* sbs_context_copy(const SbsContext *ctx);

static inline char* sbs_context_interpolate_string_realloc(SbsContext *context, char *interpolated_string)
{
    // TODO: Implement proper string interpolation
    char **keys = fl_hashtable_keys(context->evalctx->variables);

    for (size_t i=0; i < fl_array_length(keys); i++)
    {
        char *key = fl_cstring_vdup("${%s}", keys[i]);
        char *value = sbs_eval_context_get_variable(context->evalctx, keys[i]);

        if (value == NULL)
            value = "(null)";

        interpolated_string = fl_cstring_replace_realloc(interpolated_string, key, value);

        fl_cstring_free(key);
    }

    fl_array_free(keys);

    return interpolated_string;
}

static inline char* sbs_context_interpolate_string(SbsContext *context, const char *str)
{
    return sbs_context_interpolate_string_realloc(context, fl_cstring_dup(str));
}

#endif /* SBS_RUNTIME_CONTEXT_H */
