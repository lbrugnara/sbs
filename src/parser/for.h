#ifndef SBS_PARSER_FOR_H
#define SBS_PARSER_FOR_H

#include "parser.h"
#include "../objects/for.h"

typedef struct SbsSectionFor {
    SbsNodeFor *condition;
} SbsSectionFor;

SbsSectionFor* sbs_section_for_parse(SbsParser *parser);
void sbs_section_for_free(SbsSectionFor *section);
SbsSectionFor* sbs_section_for_copy(SbsSectionFor *section);
SbsSectionFor* sbs_section_for_extend(SbsSectionFor *child_section, SbsSectionFor *parent_section);

#endif /* SBS_PARSER_FOR_H */
