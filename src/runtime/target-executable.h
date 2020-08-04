#ifndef SBS_RUNTIME_TARGET_EXECUTABLE_H
#define SBS_RUNTIME_TARGET_EXECUTABLE_H

#include "target.h"
#include "resolve.h"
#include "source.h"
#include "../lang/target-executable.h"

typedef struct SbsTargetLibrary {
    char *path;
    char *name;
} SbsTargetLibrary;

typedef struct SbsTargetExecutable {
    SbsTarget base;
    char *output_name;
    SbsTargetLibrary *libraries;
    SbsSource **objects;
    char **defines;
} SbsTargetExecutable;

SbsTargetExecutable* sbs_target_executable_new(const char *name);
void sbs_target_executable_free(SbsTargetExecutable *target);
SbsTargetExecutable* sbs_target_executable_resolve(SbsResolveContext *context, const SbsSectionExecutable *executable_section);

#endif /* SBS_RUNTIME_TARGET_EXECUTABLE_H */
