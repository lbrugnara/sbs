#ifndef SBS_RESOLVE_TARGET_H
#define SBS_RESOLVE_TARGET_H

#include "../target.h"
#include "../context.h"

SbsTarget* sbs_target_resolve(SbsContext *context, const char *target_name, const SbsTarget *parent);

#endif /* SBS_RESOLVE_TARGET_H */
