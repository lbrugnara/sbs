#ifndef SBS_RESOLVE_ENVIRONMENT_H
#define SBS_RESOLVE_ENVIRONMENT_H

#include "../environment.h"
#include "../context.h"

SbsEnv* sbs_env_resolve(SbsContext *context, const char *env_name);

#endif /* SBS_RESOLVE_ENVIRONMENT_H */
