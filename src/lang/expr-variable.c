#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-variable.h"

void sbs_expr_free_variable(SbsVariableExpr *var_node)
{
    if (var_node->name)
        fl_cstring_free(var_node->name);

    if (var_node->namespace)
        fl_cstring_free(var_node->namespace);

    if (var_node->fqn)
        fl_cstring_free(var_node->fqn);

    fl_free(var_node);
}

SbsVariableExpr* sbs_expr_copy_variable(SbsVariableExpr* node)
{
    SbsVariableExpr *copy = fl_malloc(sizeof(SbsVariableExpr));

    copy->kind = SBS_EXPR_VARIABLE;
    copy->name = fl_cstring_dup(node->name);
    copy->namespace = node->namespace != NULL ? fl_cstring_dup(node->namespace) : NULL;
    copy->fqn = fl_cstring_dup(node->fqn);

    return copy;
}

SbsVariableExpr* sbs_expr_make_variable(const char *name, const char *namespace)
{
    SbsVariableExpr *var_node = fl_malloc(sizeof(SbsVariableExpr));

    var_node->kind = SBS_EXPR_VARIABLE;
    var_node->name = name;
    var_node->namespace = namespace;

    if (namespace != NULL)
    {
        var_node->fqn = fl_cstring_vdup("%s.%s", var_node->namespace, var_node->name);
    }
    else
    {
        var_node->fqn = fl_cstring_dup(var_node->name);
    }

    return var_node;
}

SbsVariableExpr* sbs_expr_parse_variable(SbsParser *parser)
{
    const SbsToken *var_token = sbs_parser_consume(parser, SBS_TOKEN_VARIABLE);
    
    SbsVariableExpr *variable = NULL;

    for (size_t i = var_token->value.length - 1; true; )
    {
        if (((const char*) var_token->value.sequence)[i] == '.')
        {
            const struct FlSlice name = fl_slice_new(var_token->value.sequence, 1, i + 1, var_token->value.length - (i + 1));
            const struct FlSlice namespace = fl_slice_new(var_token->value.sequence, 1, 0, i);
            variable = sbs_expr_make_variable(sbs_slice_to_cstring(&name), sbs_slice_to_cstring(&namespace));
            break;
        }
        else if (i == 0)
        {
            variable = sbs_expr_make_variable(sbs_slice_to_cstring(&var_token->value), NULL);
            break;
        }
        i--;
    }

    return variable;
}
