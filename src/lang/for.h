#ifndef SBS_LANG_FOR_H
#define SBS_LANG_FOR_H

#include "../runtime/eval.h"

typedef struct SbsSectionFor {
    SbsExpression *expr;
} SbsSectionFor;

SbsSectionFor* sbs_section_for_new(void);
SbsSectionFor* sbs_section_for_copy(SbsSectionFor *section);
SbsSectionFor* sbs_section_for_merge(SbsSectionFor *child_section, SbsSectionFor *parent_section);
void sbs_section_for_free(SbsSectionFor *section);

#endif /* SBS_LANG_FOR_H */
