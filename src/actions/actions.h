#ifndef SBS_ACTIONS_H
#define SBS_ACTIONS_H

#include "../executor.h"
#include "../parser/environment.h"
#include "../parser/file.h"
#include "../parser/preset.h"
#include "../objects/target.h"


enum ActionType {
    ACTION_BEFORE,
    ACTION_AFTER
};

bool sbs_actions_run(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnvSection *env, struct SbsStringOrId *actions);
bool sbs_actions_env_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, const struct SbsEnvSection *env);
bool sbs_actions_target_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnvSection *env, struct SbsTarget *target);
bool sbs_actions_preset_run(enum ActionType type, SbsExecutor executor, const struct SbsFile *file, struct SbsEnvSection *env, struct SbsPresetSection *preset);

#endif /* SBS_ACTIONS_H */
