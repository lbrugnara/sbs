#include <stdlib.h>
#include <stdio.h>
#include <fllib/Cstring.h>
#include "result.h"

static char *reason[] = {
    [SBS_RES_OK] =                      "Ok\n",
    [SBS_RES_ERROR] =                   "An unknown error ocurred\n",
    [SBS_RES_WRONG_ARGS] =              "The command expects a different set of arguments\n",
    [SBS_RES_CWD] =                     "Could not change the working directory\n",

    [SBS_RES_INVALID_RESOURCE] =        "The resource '%s' does not exist\n",
    [SBS_RES_MISSING_RESOURCE_ARG] =    "A resource name is required for this command to run\n",

    [SBS_RES_INVALID_PRESET] =          "The preset '%s' does not exist\n",
    [SBS_RES_INVALID_ENV] =             "The environemnt '%s' does not exist\n",
    [SBS_RES_INVALID_TOOLCHAIN] =       "The toolchaing '%s' does not exist\n",
    [SBS_RES_INVALID_CONFIG] =          "The configuration '%s' does not exist\n",
    [SBS_RES_INVALID_TARGET] =          "The target '%s' does not exist\n",
    [SBS_RES_INVALID_FILE] =            "Build file '%s' does not exist\n",

    [SBS_RES_MISMATCH_ENV] =            "There's no environment for the current OS and processor architecture\n",
    [SBS_RES_MISMATCH_TOOLCHAIN] =      "There's no toolchain for the current environment\n",
    [SBS_RES_MISMATCH_CONFIG] =         "There's no configuration for the current toolchain\n",
    [SBS_RES_MISMATCH_TARGET] =         "There's no target\n",

    [SBS_RES_MISSING_ENV_ARG] =         "An environment name is required for this command to run\n",
    [SBS_RES_MISSING_TOOLCHAIN_ARG] =   "A toolchain name is required for this command to run\n",
    [SBS_RES_MISSING_CONFIG_ARG] =      "A configuration name is required for this command to run\n",
    [SBS_RES_MISSING_TARGET_ARG] =      "A target name is required for this command to run\n",

    [SBS_RES_EXECUTOR] =                "Couldn't start the environment's shell.\n",

    [SBS_RES_UNK_OS] =                  "sbs is not supported on this operating system\n",
    [SBS_RES_UNK_ARCH] =                "sbs does not support the current processor architecture\n",

    [SBS_RES_ACTION_FAILED] =           "An action has failed\n",
    [SBS_RES_TARGET_FAILED] =           "Target %s has failed\n"
};

const char* sbs_result_get_reason(SbsResult result, ...)
{
if (result < SBS_RES_OK || result > SBS_RES_END)
        return NULL;

    va_list args;
    va_start(args, result);
    char *str = fl_cstring_vadup(reason[result], args);
    va_end(args);
    return str;
}

SbsResult sbs_result_print_reason(SbsResult result, ...)
{
    if (result < SBS_RES_OK || result > SBS_RES_END)
        return result;

    va_list args;
    va_start(args, result);
    char *str = fl_cstring_vadup(reason[result], args);
    va_end(args);

    fputs(str, stderr);

    fl_cstring_free(str);

    return result;
}
