#include <stddef.h>
#include <fllib/Cstring.h>
#include "for.h"
#include "../utils.h"
#include "../runtime/eval.h"

void sbs_section_for_free(SbsSectionFor *section)
{
    sbs_eval_node_free(section->condition);
    fl_free(section);
}

SbsSectionFor* sbs_section_for_copy(SbsSectionFor *section)
{
    SbsSectionFor *copy = fl_malloc(sizeof(SbsSectionFor));

    copy->condition = sbs_eval_node_copy(section->condition);

    return copy;
}

SbsSectionFor* sbs_section_for_extend(SbsSectionFor *child_section, SbsSectionFor *parent_section)
{
    if (!parent_section)
        return child_section;

    if (child_section == NULL)
    {
        child_section = sbs_section_for_copy(parent_section);
    }
    else
    {
        child_section->condition = sbs_eval_node_make_binary(SBS_EVAL_OP_AND, child_section->condition, sbs_eval_node_copy(parent_section->condition));
    }

    return child_section;
}
