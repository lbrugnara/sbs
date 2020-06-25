#ifndef SBS_EXECUTOR_H
#define SBS_EXECUTOR_H

#include <fllib.h>
#include "objects/environment.h"

typedef struct SbsExecutor SbsExecutor;

SbsExecutor* sbs_executor_new(SbsEnv *env);
bool sbs_executor_run_command(const SbsExecutor *executor, const char *command);
void sbs_executor_free(SbsExecutor *executor);

#endif /* SBS_EXECUTOR_H */
