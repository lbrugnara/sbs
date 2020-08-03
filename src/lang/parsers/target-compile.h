#ifndef SBS_PARSER_TARGET_COMPILE_H
#define SBS_PARSER_TARGET_COMPILE_H

#include "parser.h"
#include "../target-compile.h"

void sbs_section_compile_body_parse(SbsParser *parser, SbsSectionCompile *target_section, SbsNodeCompile *target);
SbsAbstractSectionTarget* sbs_section_compile_parse(SbsParser *parser);

#endif /* SBS_PARSER_TARGET_COMPILE_H */
