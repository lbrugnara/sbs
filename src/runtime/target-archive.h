#ifndef SBS_RUNTIME_TARGET_ARCHIVE_H
#define SBS_RUNTIME_TARGET_ARCHIVE_H

#include "target.h"
#include "resolve.h"
#include "source.h"
#include "../lang/target-archive.h"

typedef struct SbsTargetArchive {
    SbsTarget base;
    char *output_name;
    SbsSource **objects;
} SbsTargetArchive;

SbsTargetArchive* sbs_target_archive_new(const char *name);
void sbs_target_archive_free(SbsTargetArchive *target);
SbsTargetArchive* sbs_target_archive_resolve(SbsResolveContext *context, const SbsSectionArchive *archive_section);

#endif /* SBS_RUNTIME_TARGET_ARCHIVE_H */
