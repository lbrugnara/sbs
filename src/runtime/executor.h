#ifndef SBS_RUNTIME_EXECUTOR_H
#define SBS_RUNTIME_EXECUTOR_H

typedef struct SbsExecutor SbsExecutor;

SbsExecutor* sbs_executor_new(char *typestr, char **args, char **variables, char *terminal);
void sbs_executor_enable_script_mode(SbsExecutor *executor);
bool sbs_executor_is_script_mode(SbsExecutor *executor);
bool sbs_executor_run_command(const SbsExecutor *executor, const char *command);
void sbs_executor_free(SbsExecutor *executor);

#endif /* SBS_RUNTIME_EXECUTOR_H */
