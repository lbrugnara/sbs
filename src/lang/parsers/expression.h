#ifndef SBS_PARSER_EXPRESSION_H
#define SBS_PARSER_EXPRESSION_H

#include "parser.h"
#include "../../runtime/eval.h"

SbsExpression* sbs_expression_parse(SbsParser *parser);
SbsExpression* sbs_statement_for_parse(SbsParser *parser);
SbsExpression* sbs_statement_if_parse(SbsParser *parser);

#endif /* SBS_PARSER_EXPRESSION_H */
