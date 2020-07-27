#include <stdio.h>
#include <fllib/Array.h>
#include "build.h"
#include "archive.h"
#include "compile.h"
#include "executable.h"
#include "shared.h"
#include "action.h"
#include "../runtime/context.h"
#include "../runtime/triplet.h"
#include "../runtime/resolvers/target.h"

static inline bool resolve_target(SbsContext *context, const char *targetarg, SbsResult *result)
{
    if (targetarg == NULL)
    {
        // If the user did not provide a target name, the "targets" property within the preset must be
        // valid and it must contain at least 1 target
        if (context->preset == NULL || context->preset->targets == NULL || fl_array_length(context->preset->targets) == 0)
        {
            *result = sbs_result_print_reason(SBS_RES_MISSING_TARGET_ARG);
            return false;
        }

        context->targets = fl_array_new(sizeof(SbsTarget*), fl_array_length(context->preset->targets));
        for (size_t i = 0; i < fl_array_length(context->preset->targets); i++)
        {
            char *target_name = context->preset->targets[i];

            // Resolve target
            SbsTarget *tmp = sbs_target_resolve(context, target_name, NULL);
            if (tmp == NULL)
            {
                *result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, target_name);
                return false;
            }   

            context->targets[i] = tmp;
        }
    }
    else
    {
        SbsTarget *tmp = sbs_target_resolve(context, targetarg, NULL);
        if (tmp == NULL)
        {
            *result = sbs_result_print_reason(SBS_RES_INVALID_TARGET, targetarg);
            return false;
        }

        context->targets = fl_array_new(sizeof(SbsTarget*), 1);
        context->targets[0] = tmp;
        fl_hashtable_add(context->symbols->variables, "sbs.target", targetarg);
    }

    return true;
}

char** sbs_build_target(SbsBuild *build)
{
    char **result = NULL;

    if (build->current_target->type == SBS_TARGET_COMPILE)
    {
        result = sbs_build_compile(build);
    }
    else if (build->current_target->type == SBS_TARGET_ARCHIVE)
    {
        result = sbs_build_target_archive(build);
    }
    else if (build->current_target->type == SBS_TARGET_SHARED)
    {
        result = sbs_build_target_shared(build);
    }
    else if (build->current_target->type == SBS_TARGET_EXECUTABLE)
    {
        result = sbs_build_target_executable(build);
    }

    return result;
}

SbsResult sbs_build_run(const SbsFile *file, SbsBuildArgs *args)
{
    // We track the build command result with this variable
    SbsResult result = SBS_RES_OK;

    // Get all the available combinations of environments, toolchains, and configurations.
    SbsTriplet **triplets = sbs_triplet_find(file, args->preset, args->env, args->toolchain, args->config, args->script_mode);

    if (triplets == NULL || fl_array_length(triplets) == 0)
    {
        result = SBS_RES_ERROR;
        goto error_before_targets;
    }

    // TODO: By now, we assume the first triplet is the correct one
    SbsTriplet *triplet = triplets[0];

    // Resolve all the targets
    if (!resolve_target(triplet->context, args->target, &result))
        goto error_before_targets;

    // Create the build object needed by the build process
    SbsBuild build = {
        .context = triplet->context,
        .script_mode = args->script_mode
    };

    // At this point we resolved all the resources needed to run the build, so
    // we start executing the build actions:
    result = sbs_build_run_preset_actions(&build, SBS_BUILD_ACTION_BEFORE);

    if (result != SBS_RES_OK)
        goto error_before_targets;

    result = sbs_build_run_env_actions(&build, SBS_BUILD_ACTION_BEFORE);

    if (result != SBS_RES_OK)
        goto error_before_targets;

    // Run the build: we assume targets are not dependant between each other. Dependencies must be explicit
    // in the targets, not in presets
    SbsResult *targets_result = fl_array_new(sizeof(SbsResult), fl_array_length(build.context->targets));
    for (size_t i=0; i < fl_array_length(build.context->targets); i++)
    {
        build.current_target = build.context->targets[i];

        fl_hashtable_add(build.context->symbols->variables, "sbs.target", build.current_target->name);

        targets_result[i] = sbs_build_run_target_actions(&build, SBS_BUILD_ACTION_BEFORE);

        if (targets_result[i] != SBS_RES_OK)
            continue;

        char **target_output = sbs_build_target(&build);
        
        fl_hashtable_remove(build.context->symbols->variables, "sbs.target", true, true);

        if (target_output == NULL)
        {
            targets_result[i] = SBS_RES_TARGET_FAILED;
            continue;
        }

        targets_result[i] = sbs_build_run_target_actions(&build, SBS_BUILD_ACTION_AFTER);
        fl_array_free_each_pointer(target_output, (FlArrayFreeElementFunc) fl_cstring_free);
    }

    for (size_t i = 0; i < fl_array_length(targets_result); i++)
    {
        SbsResult target_result = targets_result[i];

        if (target_result == SBS_RES_OK)
            continue;

        result = SBS_RES_TARGET_FAILED;

        // TODO: Fix this
        fprintf(stderr, "Target %s failed.\n", build.context->targets[i]->name);
    }

    SbsResult action_result = sbs_build_run_env_actions(&build, SBS_BUILD_ACTION_AFTER);

    if (result == SBS_RES_OK && action_result != SBS_RES_OK)
        result = action_result;

    if (result != SBS_RES_OK)
        goto error_after_targets;

    action_result = sbs_build_run_preset_actions(&build, SBS_BUILD_ACTION_AFTER);

    if (result == SBS_RES_OK && action_result != SBS_RES_OK)
        result = action_result;

error_after_targets:
    if (targets_result != NULL)
        fl_array_free(targets_result);

error_before_targets:
    if (triplets != NULL)
        fl_array_free_each_pointer(triplets, (FlArrayFreeElementFunc) sbs_triplet_free);

    return result;
}
