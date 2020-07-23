#ifndef SBS_BUILD_ACTION_H
#define SBS_BUILD_ACTION_H

#include "../result.h"
#include "../context.h"

typedef enum SbsBuildActionType {
    SBS_BUILD_ACTION_BEFORE,
    SBS_BUILD_ACTION_AFTER
} SbsBuildActionType;

SbsResult sbs_build_action_run(SbsContext *context, SbsBuildActionType type, bool run_target);

#endif /* SBS_BUILD_ACTION_H */
