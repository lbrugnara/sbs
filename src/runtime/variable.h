#ifndef SBS_RUNTIME_VARIABLE_H
#define SBS_RUNTIME_VARIABLE_H

#include <fllib/Slice.h>

typedef struct SbsVariableInfo {
    const char *namespace;
    const char *name;
    const char *fqn;
} SbsVariableInfo;

SbsVariableInfo* sbs_varinfo_new(const char *name, const char *namespace);
void sbs_varinfo_free(SbsVariableInfo *varinfo);
SbsVariableInfo* sbs_varinfo_copy(const SbsVariableInfo *varinfo);

#endif /* SBS_RUNTIME_VARIABLE_H */
