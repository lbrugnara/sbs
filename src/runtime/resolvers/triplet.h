#ifndef SBS_RESOLVE_TRIPLET_H
#define SBS_RESOLVE_TRIPLET_H

#include "../triplet.h"
#include "../context.h"

FlList* sbs_triplet_resolve_all(SbsContext *context, const char *env, const char *toolchain, const char *config, bool script_mode);

#endif /* SBS_RESOLVE_TRIPLET_H */
