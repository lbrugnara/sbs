#ifndef SBS_RESOLVE_CONFIGURATION_H
#define SBS_RESOLVE_CONFIGURATION_H

#include "../configuration.h"
#include "../context.h"

SbsConfiguration* sbs_config_resolve(SbsContext *context, const char *config_name);

#endif /* SBS_RESOLVE_CONFIGURATION_H */
