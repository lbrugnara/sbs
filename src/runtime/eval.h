#ifndef SBS_RUNTIME_EVAL_H
#define SBS_RUNTIME_EVAL_H

#include <fllib/containers/Hashtable.h>
#include "../lang/expression.h"

typedef struct SbsEvalContext {
    FlHashtable *variables;
} SbsEvalContext;

SbsEvalContext* sbs_eval_context_new(void);
void sbs_eval_context_free(SbsEvalContext *context);
SbsValueExpr* sbs_expression_eval(SbsEvalContext *context, SbsExpression *node);
bool sbs_expression_eval_bool(SbsEvalContext *context, SbsExpression *node);
char* sbs_expression_eval_string(struct SbsEvalContext *context, SbsString *string);

#endif /* SBS_RUNTIME_EVAL_H */
