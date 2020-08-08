#ifndef SBS_LANG_EXPR_STRING_H
#define SBS_LANG_EXPR_STRING_H

#include "expr.h"
#include "expr-variable.h"

typedef struct SbsStringPlaceholder {
SbsVariableExpr *variable;
size_t index;
} SbsStringPlaceholder;

typedef struct SbsStringExpr {
SbsExpressionKind kind;
const char *format;
SbsStringPlaceholder **args;
bool is_constant;
} SbsStringExpr;

SbsStringExpr* sbs_expr_parse_string(SbsParser *parser);
SbsStringExpr** sbs_expr_parse_string_array(SbsParser *parser);
SbsStringExpr* sbs_expr_make_string(char *format, SbsStringPlaceholder **placeholders);
void sbs_expr_free_string(SbsStringExpr *string);
SbsStringExpr* sbs_expr_copy_string(const SbsStringExpr *string);

static inline SbsStringExpr** sbs_expr_extend_string_array(SbsStringExpr **dest, SbsStringExpr **source)
{    
return sbs_array_extend_copy_pointer(dest, source, (SbsArrayCopyPointerFn) sbs_expr_copy_string);
}

static inline SbsStringExpr* sbs_expr_set_string(SbsStringExpr *dest, const SbsStringExpr *src)
{
if (!src)
    return dest;

if (!dest)
{
    dest = sbs_expr_copy_string(src);
    return dest;
}

sbs_expr_free_string(dest);
dest = sbs_expr_copy_string(src);
return dest;
}

#endif /* SBS_LANG_EXPR_STRING_H */
