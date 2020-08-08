#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-identifier.h"

void sbs_expr_free_identifier(SbsIdentifierExpr *identifier)
{
    fl_cstring_free(identifier->name);
    fl_free(identifier);
}

SbsIdentifierExpr* sbs_expr_copy_identifier(SbsIdentifierExpr *identifier)
{
    SbsIdentifierExpr *copy = fl_malloc(sizeof(SbsIdentifierExpr));

    copy->kind = SBS_EXPR_IDENTIFIER;
    copy->name = fl_cstring_dup(identifier->name);

    return copy;
}

SbsIdentifierExpr* sbs_expr_make_identifier(char *id)
{
    SbsIdentifierExpr *id_expr = fl_malloc(sizeof(SbsIdentifierExpr));

    id_expr->kind = SBS_EXPR_IDENTIFIER;
    id_expr->name = id;

    return id_expr;
}

SbsIdentifierExpr* sbs_expr_parse_identifier(SbsParser *parser)
{
    const SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    
    // TODO: We assume strings here...
    SbsIdentifierExpr *id_expr = sbs_expr_make_identifier(sbs_token_to_str(id));

    return id_expr;
}
