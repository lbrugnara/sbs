#ifndef SBS_ACTIONS_H
#define SBS_ACTIONS_H

#include "../executor.h"
#include "../objects/environment.h"
#include "../objects/file.h"
#include "../objects/preset.h"
#include "../objects/target.h"

enum ActionType {
    ACTION_BEFORE,
    ACTION_AFTER
};

bool sbs_actions_run(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, struct SbsStringOrId *actions);
bool sbs_actions_env_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env);
bool sbs_actions_target_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target);
bool sbs_actions_preset_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsPreset *preset);

#endif /* SBS_ACTIONS_H */
