#include <fllib/Mem.h>
#include "triplet.h"
#include "resolvers/preset.h"
#include "resolvers/triplet.h"

SbsTriplet* sbs_triplet_new(SbsContext *context)
{
    SbsTriplet *triplet = fl_malloc(sizeof(SbsTriplet));

    triplet->context = context;

    return triplet;
}

void sbs_triplet_free(SbsTriplet *triplet)
{
    if (triplet->context)
        sbs_context_free(triplet->context);

    fl_free(triplet);
}

static inline bool resolve_executor(SbsContext *context, bool script_mode)
{
    context->executor = sbs_executor_new(context->env->type, context->env->args, context->env->variables, context->env->terminal);

    if (context->executor == NULL)
        return false;

    if (script_mode)
        sbs_executor_enable_script_mode(context->executor);

    return true;
}

static inline bool resolve_preset(SbsContext *context, const char *preset_name)
{
    context->preset = sbs_preset_resolve(context, preset_name);

    return context->preset != NULL;
}

SbsTriplet** sbs_triplet_find(const SbsFile *file, const char *preset, const char *env, const char *toolchain, const char *config, bool script_mode)
{
    // Our return value
    SbsTriplet **triplets_array = NULL;

    // Create a context object to resolve the triplets
    SbsContext *context = sbs_context_new(file);

    // Check if the build uses a preset and in that case make sure it is a valid preset
    if (preset != NULL && !resolve_preset(context, preset))
        goto preset_error;

    triplets_array = fl_array_new(sizeof(SbsTriplet*), 0);
    FlList *triplets_list = sbs_triplet_resolve_all(context, env, toolchain, config, script_mode);

    struct FlListNode *node = fl_list_head(triplets_list);
    while (node)
    {
        SbsTriplet *triplet = (SbsTriplet*) node->value;

        if (!resolve_executor(triplet->context, script_mode))
        {
            sbs_triplet_free(triplet);
            goto next_node;
        }
        
        triplets_array = fl_array_append(triplets_array, &triplet);

next_node:
        node = node->next; // Update for next iteration
    }

    fl_list_free(triplets_list);

preset_error:
    if (context)
        sbs_context_free(context);

    return triplets_array;
}
