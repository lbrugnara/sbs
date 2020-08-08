#ifndef SBS_PARSER_EXPRESSION_H
#define SBS_PARSER_EXPRESSION_H

#include "../parser.h"
#include "../expression.h"

SbsExpression* sbs_expression_parse(SbsParser *parser);
SbsExpression* sbs_statement_for_parse(SbsParser *parser);
SbsExpression* sbs_statement_if_parse(SbsParser *parser);
SbsIdentifierExpr* sbs_expression_identifier_parse(SbsParser *parser);
SbsVariableExpr* sbs_expression_variable_parse(SbsParser *parser);
SbsVarDefinitionExpr* sbs_expression_var_definition_parse(SbsParser *parser);
SbsArrayExpr* sbs_expression_array_parse(SbsParser *parser);
SbsStringExpr* sbs_expression_string_parse(SbsParser *parser);
SbsExpression* sbs_expression_primary_parse(SbsParser *parser);
SbsExpression* sbs_expression_unary_parse(SbsParser *parser);
SbsExpression* sbs_expression_comparisson_parse(SbsParser *parser);
SbsExpression* sbs_expression_equality_parse(SbsParser *parser);
SbsExpression* sbs_expression_and_parse(SbsParser *parser);
SbsExpression* sbs_expression_or_parse(SbsParser *parser);
SbsExpression* sbs_expression_conditional_parse(SbsParser *parser);

#endif /* SBS_PARSER_EXPRESSION_H */
