#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr.h"
#include "expr-array.h"
#include "expr-binary.h"
#include "expr-identifier.h"
#include "expr-if.h"
#include "expr-string.h"
#include "expr-unary.h"
#include "expr-value.h"
#include "expr-vardef.h"
#include "expr-variable.h"

void sbs_expr_free(SbsExpression *node)
{
    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            sbs_expr_free_value((SbsValueExpr*) node);
            break;

        case SBS_EXPR_VARIABLE:
            sbs_expr_free_variable((SbsVariableExpr*) node);
            break;

        case SBS_EXPR_ARRAY:
            sbs_expr_free_array((SbsArrayExpr*) node);
            break;

        case SBS_EXPR_UNARY:
            sbs_expr_free_unary((SbsUnaryExpr*) node);
            break;

        case SBS_EXPR_BINARY:
            sbs_expr_free_binary((SbsBinaryExpr*) node);
            break;

        case SBS_EXPR_IF:
            sbs_expr_free_if((SbsIfExpr*) node);
            break;

        case SBS_EXPR_STRING:
            sbs_expr_free_string((SbsStringExpr*) node);
            break;

        case SBS_EXPR_IDENTIFIER:
            sbs_expr_free_identifier((SbsIdentifierExpr*) node);
            break;

        case SBS_EXPR_VAR_DEFINITION:
            sbs_expr_free_vardef((SbsVarDefinitionExpr*) node);
            break;

        default:
            return;
    }
}

SbsExpression* sbs_expr_copy(SbsExpression *node)
{
    switch (node->kind)
    {
        case SBS_EXPR_VALUE:
            return (SbsExpression*) sbs_expr_copy_value((SbsValueExpr*) node);

        case SBS_EXPR_VARIABLE:
            return (SbsExpression*) sbs_expr_copy_variable((SbsVariableExpr*) node);

        case SBS_EXPR_ARRAY:
            return (SbsExpression*) sbs_expr_copy_array((SbsArrayExpr*) node);

        case SBS_EXPR_UNARY:
            return (SbsExpression*) sbs_expr_copy_unary((SbsUnaryExpr*) node);

        case SBS_EXPR_BINARY:
            return (SbsExpression*) sbs_expr_copy_binary((SbsBinaryExpr*) node);

        case SBS_EXPR_IF:
            return (SbsExpression*) sbs_expr_copy_if((SbsIfExpr*) node);

        case SBS_EXPR_STRING:
            return (SbsExpression*) sbs_expr_copy_string((SbsStringExpr*) node);

        case SBS_EXPR_IDENTIFIER:
            return (SbsExpression*) sbs_expr_copy_identifier((SbsIdentifierExpr*) node);

        case SBS_EXPR_VAR_DEFINITION:
            return (SbsExpression*) sbs_expr_copy_vardef((SbsVarDefinitionExpr*) node);

        default:
            break;
    }

    return NULL;
}

SbsExpression* sbs_expr_parse(SbsParser *parser)
{
    return sbs_expr_parse_conditional(parser);
}

SbsExpression* sbs_expr_parse_for(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_FOR);
    return sbs_expr_parse(parser);
}

