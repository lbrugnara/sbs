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

typedef struct SbsContextArgs {
    char *env;
    char *toolchain;
    char *config;
    char *target;
    char *preset;
    bool script_mode;
} SbsContextArgs;

typedef struct SbsContext {
    const SbsFile *file;
    SbsHostInfo *host;
    SbsEvalContext *symbols;
    SbsExecutor *executor;
    SbsEnv *env;
    SbsToolchain *toolchain;
    SbsTarget **targets;
    SbsConfiguration *config;
    SbsPreset *preset;
} SbsContext;

SbsContext* sbs_context_new(const SbsFile *file, SbsContextArgs *args, enum SbsResult *result);
void sbs_context_free(SbsContext *context);
SbsContext* sbs_context_copy(const SbsContext *ctx);
SbsContext* sbs_context_copy_args(const SbsContext *ctx, SbsContextArgs *args);

inline char* sbs_context_interpolate_string_realloc(SbsContext *context, char *interpolated_string)
{
    // TODO: Implement proper string interpolation
    char **keys = fl_hashtable_keys(context->symbols->variables);

    for (size_t i=0; i < fl_array_length(keys); i++)
    {
        char *key = fl_cstring_vdup("${%s}", keys[i]);

        interpolated_string = fl_cstring_replace_realloc(interpolated_string, key, (char*) fl_hashtable_get(context->symbols->variables, keys[i]));

        fl_cstring_free(key);
    }

    fl_array_free(keys);

    return interpolated_string;
}

inline char* sbs_context_interpolate_string(SbsContext *context, const char *str)
{
    return sbs_context_interpolate_string_realloc(context, fl_cstring_dup(str));
}

#endif /* SBS_RUNTIME_CONTEXT_H */
