#ifndef SBS_PARSER_TARGET_EXECUTABLE_H
#define SBS_PARSER_TARGET_EXECUTABLE_H

#include "parser.h"
#include "../target-executable.h"

void sbs_section_executable_body_parse(SbsParser *parser, SbsSectionExecutable *target_section, SbsNodeExecutable *target);
SbsAbstractSectionTarget* sbs_section_executable_parse(SbsParser *parser);

#endif /* SBS_PARSER_TARGET_EXECUTABLE_H */
