#ifndef SBS_LANG_EXPR_STRING_H
#define SBS_LANG_EXPR_STRING_H

#include "expr.h"
#include "string.h"

typedef struct SbsStringExpr {
    SbsExpressionKind kind;
    struct SbsString *value;
} SbsStringExpr;

SbsStringExpr* sbs_expr_parse_string(SbsParser *parser);
SbsStringExpr* sbs_expr_make_string(struct SbsString *string);
void sbs_expr_free_string(SbsStringExpr *string);
SbsStringExpr* sbs_expr_copy_string(SbsStringExpr *string);

static inline SbsStringExpr** sbs_expr_extend_string_array(SbsStringExpr **dest, SbsStringExpr **source)
{    
    return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_expr_copy_string);
}

#endif /* SBS_LANG_EXPR_STRING_H */
