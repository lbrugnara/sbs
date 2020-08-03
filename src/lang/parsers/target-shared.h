#ifndef SBS_PARSER_TARGET_SHARED_H
#define SBS_PARSER_TARGET_SHARED_H

#include "parser.h"
#include "../target-shared.h"

void sbs_section_shared_body_parse(SbsParser *parser, SbsSectionShared *target_section, SbsNodeShared *target);
SbsAbstractSectionTarget* sbs_section_shared_parse(SbsParser *parser);

#endif /* SBS_PARSER_TARGET_SHARED_H */
