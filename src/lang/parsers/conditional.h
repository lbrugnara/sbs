#ifndef SBS_PARSER_STMT_CONDITIONAL_H
#define SBS_PARSER_STMT_CONDITIONAL_H

#include "parser.h"
#include "../conditional.h"

SbsExpression* sbs_expr_conditional_parse(SbsParser *parser);
SbsStmtConditional* sbs_stmt_conditional_parse(SbsParser *parser);

#endif /* SBS_PARSER_STMT_CONDITIONAL_H */
