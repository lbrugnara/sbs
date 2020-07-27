#ifndef SBS_LANG_STMT_CONDITIONAL_H
#define SBS_LANG_STMT_CONDITIONAL_H

#include "../runtime/eval.h"

typedef struct SbsStmtConditional {
    SbsExpression *expr;
} SbsStmtConditional;

SbsStmtConditional* sbs_stmt_conditional_new(void);
SbsStmtConditional* sbs_stmt_conditional_copy(SbsStmtConditional *section);
SbsStmtConditional* sbs_stmt_conditional_merge(SbsStmtConditional *child_section, SbsStmtConditional *parent_section);
void sbs_stmt_conditional_free(SbsStmtConditional *section);

#endif /* SBS_LANG_STMT_CONDITIONAL_H */
