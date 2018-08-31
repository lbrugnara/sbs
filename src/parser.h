#ifndef SBS_PARSER_H
#define SBS_PARSER_H

#include "file.h"
#include "lexer.h"

bool sbs_parser_parse(struct SbsLexer *lexer, struct SbsFile *file);

#endif
