#include <stdio.h>
#include <fllib.h>

#include "build.h"
#include "action.h"
#include "archive.h"
#include "compile.h"
#include "executable.h"
#include "shared.h"
#include "../context.h"
#include "../utils.h"
#include "../host.h"
#include "../executor.h"
#include "../parser/file.h"
#include "../parser/action.h"
#include "../parser/helpers.h"
#include "../parser/configuration.h"
#include "../parser/target.h"
#include "../parser/toolchain.h"
#include "../parser/lexer.h"
#include "../parser/parser.h"
#include "../parser/file.h"

#include "../objects/configuration.h"
#include "../objects/toolchain.h"
#include "../objects/preset.h"

char** sbs_build_target(SbsBuild *build)
{
    if (build->context->target->type == SBS_TARGET_COMPILE)
    {
        return sbs_build_compile(build);
    }
    else if (build->context->target->type == SBS_TARGET_ARCHIVE)
    {
        return sbs_build_target_archive(build);
    }
    else if (build->context->target->type == SBS_TARGET_SHARED)
    {
        return sbs_build_target_shared(build);
    }
    else if (build->context->target->type == SBS_TARGET_EXECUTABLE)
    {
        return sbs_build_target_executable(build);
    }

    return NULL;
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
    result = sbs_build_action_run(build.context, SBS_BUILD_ACTION_BEFORE, true);

    if (result != SBS_RES_OK)
        goto error;

    // Run the build
    char **target_output = sbs_build_target(&build);

    // We run the target actions only if the build has been successful
    result = sbs_build_action_run(build.context, SBS_BUILD_ACTION_AFTER, target_output != NULL);

    if (target_output)
        fl_array_free_each_pointer(target_output, (FlArrayFreeElementFunc) fl_cstring_free);

error:
    sbs_context_free(build.context);

    return result;
}
