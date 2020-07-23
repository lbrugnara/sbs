#ifndef SBS_LANG_FOR_H
#define SBS_LANG_FOR_H

#include "../runtime/eval.h"

typedef struct SbsSectionFor {
    SbsEvalNode *condition;
} SbsSectionFor;

void sbs_section_for_free(SbsSectionFor *section);
SbsSectionFor* sbs_section_for_copy(SbsSectionFor *section);
SbsSectionFor* sbs_section_for_extend(SbsSectionFor *child_section, SbsSectionFor *parent_section);

#endif /* SBS_LANG_FOR_H */
