#include <stddef.h>
#include <fllib/Cstring.h>
#include "conditional.h"
#include "../utils.h"
#include "../runtime/eval.h"

SbsStmtConditional* sbs_stmt_conditional_new(void)
{
    SbsStmtConditional *for_section = fl_malloc(sizeof(SbsStmtConditional));

    return for_section;
}

void sbs_stmt_conditional_free(SbsStmtConditional *section)
{
    sbs_expression_free(section->expr);
    fl_free(section);
}

SbsStmtConditional* sbs_stmt_conditional_copy(SbsStmtConditional *section)
{
    SbsStmtConditional *copy = fl_malloc(sizeof(SbsStmtConditional));

    copy->expr = sbs_expression_copy(section->expr);

    return copy;
}

SbsStmtConditional* sbs_stmt_conditional_merge(SbsStmtConditional *child_section, SbsStmtConditional *parent_section)
{
    if (!parent_section)
        return child_section;

    if (child_section == NULL)
    {
        child_section = sbs_stmt_conditional_copy(parent_section);
    }
    else
    {
        child_section->expr = (SbsExpression*) sbs_expression_make_binary(SBS_EVAL_OP_AND, child_section->expr, sbs_expression_copy(parent_section->expr));
    }

    return child_section;
}
