#ifndef SBS_RUNTIME_EVAL_H
#define SBS_RUNTIME_EVAL_H

#include <fllib/containers/Hashtable.h>
#include "../lang/expr.h"
#include "../lang/expr-string.h"
#include "../lang/expr-value.h"

typedef struct SbsEvalContext {
    FlHashtable *variables;
} SbsEvalContext;

SbsEvalContext* sbs_eval_context_new(void);
char* sbs_eval_context_add_variable(SbsEvalContext *context, const char *name, const char *value);
char* sbs_eval_context_get_variable(SbsEvalContext *context, const char *name);
void sbs_eval_context_remove_variable(SbsEvalContext *context, const char *name);
void sbs_eval_context_free(SbsEvalContext *context);
void sbs_eval_context_merge(SbsEvalContext *dest_context, SbsEvalContext *src_context);
SbsValueExpr* sbs_eval_expr(SbsEvalContext *context, SbsExpression *node);
bool sbs_eval_expr_to_bool(SbsEvalContext *context, SbsExpression *node);
char* sbs_eval_expr_to_cstring(struct SbsEvalContext *context, SbsExpression *node);
char* sbs_eval_expr_string_to_cstring(struct SbsEvalContext *context, SbsStringExpr *node);

#endif /* SBS_RUNTIME_EVAL_H */
