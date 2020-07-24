#ifndef SBS_RESOLVE_TOOLCHAIN_H
#define SBS_RESOLVE_TOOLCHAIN_H

#include "../toolchain.h"
#include "../context.h"

SbsToolchain* sbs_toolchain_resolve(SbsContext *context, const char *toolchain_name);

#endif /* SBS_RESOLVE_TOOLCHAIN_H */
