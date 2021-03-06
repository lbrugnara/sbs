#ifndef SBS_RESULT_H
#define SBS_RESULT_H

typedef enum SbsResult {
    SBS_RES_OK = 0,
    SBS_RES_ERROR,
    SBS_RES_WRONG_ARGS,
    SBS_RES_CWD,
    SBS_RES_BAD_INPUT,

    SBS_RES_INVALID_RESOURCE,
    SBS_RES_MISSING_RESOURCE_ARG,

    SBS_RES_INVALID_PRESET,
    SBS_RES_INVALID_ENV,
    SBS_RES_INVALID_TOOLCHAIN,
    SBS_RES_INVALID_CONFIG,
    SBS_RES_INVALID_TARGET,
    SBS_RES_INVALID_FILE,

    SBS_RES_MISMATCH_ENV,
    SBS_RES_MISMATCH_TOOLCHAIN,
    SBS_RES_MISMATCH_CONFIG,
    SBS_RES_MISMATCH_TARGET,

    SBS_RES_MISSING_ENV_ARG,
    SBS_RES_MISSING_TOOLCHAIN_ARG,
    SBS_RES_MISSING_CONFIG_ARG,
    SBS_RES_MISSING_TARGET_ARG,

    SBS_RES_EXECUTOR,
    
    SBS_RES_ACTION_FAILED,
    SBS_RES_TARGET_FAILED,

    SBS_RES_UNK_OS,
    SBS_RES_UNK_ARCH,

    //
    SBS_RES_END
} SbsResult;

SbsResult sbs_result_print_reason(SbsResult result, ...);
const char* sbs_result_get_reason(SbsResult result, ...);

#endif /* SBS_RESULT_H */
