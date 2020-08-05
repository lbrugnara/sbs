#ifndef SBS_RUNTIME_EXECUTOR_H
#define SBS_RUNTIME_EXECUTOR_H

#include "environment.h"

typedef struct SbsExecutor SbsExecutor;

SbsExecutor* sbs_executor_new(SbsEnv *environment);
void sbs_executor_enable_script_mode(SbsExecutor *executor);
bool sbs_executor_is_script_mode(SbsExecutor *executor);
bool sbs_executor_run_command(const SbsExecutor *executor, const char *command);
void sbs_executor_free(SbsExecutor *executor);

#endif /* SBS_RUNTIME_EXECUTOR_H */
