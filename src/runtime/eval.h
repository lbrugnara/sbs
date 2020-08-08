#ifndef SBS_RUNTIME_EVAL_H
#define SBS_RUNTIME_EVAL_H

#include <fllib/containers/Hashtable.h>
#include "../lang/expr.h"
#include "../lang/expr-value.h"
#include "../lang/string.h"

typedef struct SbsEvalContext {
    FlHashtable *variables;
} SbsEvalContext;

SbsEvalContext* sbs_eval_context_new(void);
void sbs_eval_context_free(SbsEvalContext *context);
SbsValueExpr* sbs_eval_expression(SbsEvalContext *context, SbsExpression *node);
bool sbs_eval_bool_expression(SbsEvalContext *context, SbsExpression *node);
char* sbs_eval_string_expression(struct SbsEvalContext *context, SbsExpression *node);
char* sbs_eval_string_interpolation(struct SbsEvalContext *context, SbsString *string);

#endif /* SBS_RUNTIME_EVAL_H */
