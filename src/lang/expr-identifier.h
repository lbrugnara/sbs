#ifndef SBS_LANG_EXPR_IDENTIFIER_H
#define SBS_LANG_EXPR_IDENTIFIER_H

#include "expr.h"

typedef struct SbsIdentifierExpr {
    SbsExpressionKind kind;
    char *name;
} SbsIdentifierExpr;

SbsIdentifierExpr* sbs_expr_parse_identifier(SbsParser *parser);
SbsIdentifierExpr* sbs_expr_make_identifier(char *id);
void sbs_expr_free_identifier(SbsIdentifierExpr *identifier);
SbsIdentifierExpr* sbs_expr_copy_identifier(SbsIdentifierExpr *identifier);

#endif /* SBS_LANG_EXPR_IDENTIFIER_H */
