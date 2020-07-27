#ifndef SBS_BUILD_ACTION_H
#define SBS_BUILD_ACTION_H

#include "../result.h"
#include "build.h"

typedef enum SbsBuildActionType {
    SBS_BUILD_ACTION_BEFORE,
    SBS_BUILD_ACTION_AFTER
} SbsBuildActionType;

SbsResult sbs_build_run_env_actions(SbsBuild *build, SbsBuildActionType type);
SbsResult sbs_build_run_target_actions(SbsBuild *build, SbsBuildActionType type);
SbsResult sbs_build_run_preset_actions(SbsBuild *build, SbsBuildActionType type);

#endif /* SBS_BUILD_ACTION_H */
