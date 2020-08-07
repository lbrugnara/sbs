#ifndef SBS_PARSER_TARGET_H
#define SBS_PARSER_TARGET_H

#include "parser.h"
#include "../target.h"

SbsAbstractSectionTarget* sbs_section_target_parse(SbsParser *parser);
void sbs_section_target_if_stmt_parse(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type);

#endif /* SBS_PARSER_TARGET_H */
