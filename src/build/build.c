#include <stdio.h>
#include <fllib/Array.h>
#include "build.h"
#include "archive.h"
#include "compile.h"
#include "executable.h"
#include "shared.h"
#include "action.h"
#include "../runtime/context.h"

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

SbsResult sbs_build_run(const SbsFile *file, SbsContextArgs *args)
{
    // We track the build command result with this variable
    SbsResult result = SBS_RES_OK;

    // These are all the resources needed to run the build process
    // At this point we resolved all the resources needed to run the build
    SbsBuild build = {
        .context = sbs_context_new(file, args, &result),
        .script_mode = args->script_mode,
    };

    if (result != SBS_RES_OK)
        return result;

    // At this point we resolved all the resources needed to run the build
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
    sbs_context_free(build.context);

    return result;
}
