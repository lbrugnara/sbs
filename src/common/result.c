#include <stdlib.h>
#include <fllib.h>
#include "result.h"

static char *reason[] = {
    [SBS_RES_OK] =                      "Ok\n",
    [SBS_RES_ERROR] =                   "An unknown error ocurred\n",
    [SBS_RES_WRONG_ARGS] =              "The command expects a different set of arguments\n",

    [SBS_RES_INVALID_RESOURCE] =        "The resource '%s' does not exist\n",
    [SBS_RES_MISSING_RESOURCE_ARG] =    "A resource name is required for this command to run\n",

    [SBS_RES_INVALID_PRESET] =          "The preset '%s' does not exist\n",
    [SBS_RES_INVALID_ENV] =             "The environemnt '%s' does not exist\n",
    [SBS_RES_INVALID_TOOLCHAIN] =       "The toolchaing '%s' does not exist\n",
    [SBS_RES_INVALID_CONFIG] =          "The configuration '%s' does not exist\n",
    [SBS_RES_INVALID_TARGET] =          "The target '%s' does not exist\n",
    [SBS_RES_INVALID_FILE] =            "Build file '%s' does not exist\n",

    [SBS_RES_MISSING_ENV_ARG] =         "An environment name is required for this command to run\n",
    [SBS_RES_MISSING_TOOLCHAIN_ARG] =   "A toolchain name is required for this command to run\n",
    [SBS_RES_MISSING_CONFIG_ARG] =      "A configuration name is required for this command to run\n",
    [SBS_RES_MISSING_TARGET_ARG] =      "A target name is required for this command to run\n",

    [SBS_RES_EXECUTOR] =                "Couldn't start the environment's shell.\n",

    [SBS_RES_ACTION_FAILED] =           "An action has failed\n"
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
