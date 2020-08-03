#ifndef SBS_PARSER_TARGET_ARCHIVE_H
#define SBS_PARSER_TARGET_ARCHIVE_H

#include "parser.h"
#include "../target-archive.h"

void sbs_section_archive_body_parse(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target);
SbsAbstractSectionTarget* sbs_section_archive_parse(SbsParser *parser);

#endif /* SBS_PARSER_TARGET_ARCHIVE_H */
