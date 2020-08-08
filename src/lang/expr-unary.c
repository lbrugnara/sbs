#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "expr-unary.h"
#include "expr-binary.h"
#include "expr-array.h"
#include "expr-identifier.h"
#include "expr-string.h"
#include "expr-value.h"
#include "expr-variable.h"

static SbsExprOperator UnaryOperators[] = {
    SBS_EXPR_OP_ID,
    SBS_EXPR_OP_NOT,
};

void sbs_expr_free_unary(SbsUnaryExpr *unary_node)
{
    sbs_expr_free(unary_node->node);
    fl_free(unary_node);
}

SbsUnaryExpr* sbs_expr_copy_unary(SbsUnaryExpr* node)
{
    SbsUnaryExpr *copy = fl_malloc(sizeof(SbsUnaryExpr));

    copy->kind = SBS_EXPR_UNARY;
    copy->op = node->op;
    copy->node = sbs_expr_copy(node->node);

    return copy;
}

SbsUnaryExpr* sbs_expr_make_unary(SbsExprOperator op, SbsExpression *left)
{
    if (!fl_array_contains_n(UnaryOperators, flm_array_length(UnaryOperators), &op, sizeof(SbsExprOperator)))
        return NULL;

    SbsUnaryExpr *unary_node = fl_malloc(sizeof(SbsUnaryExpr));

    unary_node->kind = SBS_EXPR_UNARY;
    unary_node->op = op;
    unary_node->node = left;

    return unary_node;
}

static char** parse_identifiers(SbsParser *parser)
{
    // Track how many identifiers
    size_t identifiers_count = 0;
    const SbsToken *tmp;

    for (size_t i=0; sbs_parser_has_input(parser) && ((tmp = sbs_parser_peek_at(parser, i))->type == SBS_TOKEN_COMMA || tmp->type == SBS_TOKEN_IDENTIFIER); i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;

        identifiers_count++;
    }
    
    // Parse the identifiers
    char **identifiers = fl_array_new(sizeof(char*), identifiers_count);
    size_t index = 0;

    while (identifiers_count--)
    {
        const SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*) id->value.sequence, id->value.length);

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}

static SbsExpression* parse_inlist_expression(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    if (token->type == SBS_TOKEN_LPAREN)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        SbsExpression *node = sbs_expr_parse(parser);
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);
        return node;
    }

    const char *var_name = NULL;
    const char *var_namespace = NULL;
    bool is_compat_env = false;

    if (sbs_token_equals(token, "os"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "os";
        var_namespace = "sbs";
    }
    else if (sbs_token_equals(token, "arch"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "arch";
        var_namespace = "sbs";
    }
    else if (sbs_token_equals(token, "toolchain"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "toolchain";
        var_namespace = "sbs";
    }
    else if (sbs_token_equals(token, "config"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "config";
        var_namespace = "sbs";
    }
    else if (sbs_token_equals(token, "target"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "target";
        var_namespace = "sbs";
    }
    else // if (sbs_token_equals(token, "env")) For compatibility, if we find a for section without a keyword, it is tied to the environment
    {
        var_name = "env";
        var_namespace = "sbs";
        if (sbs_token_equals(token, "env"))
        {
            sbs_parser_consume(parser, token->type);
            sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        }
        else
        {
            is_compat_env = true;
        }
    }

    SbsExpression *var_node = (SbsExpression*) sbs_expr_make_variable(fl_cstring_dup(var_name), fl_cstring_dup(var_namespace));
    SbsArrayExpr *array_node = sbs_expr_make_array();

    while (sbs_parser_has_input(parser))
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if ((is_compat_env && token->type == SBS_TOKEN_LBRACE) || (!is_compat_env && token->type == SBS_TOKEN_RPAREN))
            break;

        char **identifiers = parse_identifiers(parser);

        for (size_t i=0; i < fl_array_length(identifiers); i++)
        {
            // TODO: We assume strings here...
            SbsValueExpr *str = sbs_expr_make_value(SBS_EXPR_VALUE_TYPE_STRING);
            str->value.s = identifiers[i];

            sbs_expr_array_add_item(array_node, (SbsExpression*) str);
        }

        fl_array_free(identifiers);
    }

    if (!is_compat_env)
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);

    return (SbsExpression*) sbs_expr_make_binary(SBS_EXPR_OP_IN, var_node, (SbsExpression*) array_node);
}

SbsExpression* sbs_expr_parse_primary(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    SbsExpression *node = NULL;

    if (token->type == SBS_TOKEN_VARIABLE)
    {
        node = (SbsExpression*) sbs_expr_parse_variable(parser);
    }
    else if (token->type == SBS_TOKEN_IDENTIFIER)
    {
        node = (SbsExpression*) sbs_expr_parse_identifier(parser);
    }
    else if (token->type == SBS_TOKEN_STRING || token->type == SBS_TOKEN_COMMAND_STRING)
    {
        node = (SbsExpression*) sbs_expr_parse_string(parser);
    }
    else if (token->type == SBS_TOKEN_LBRACKET)
    {
        node = (SbsExpression*) sbs_expr_parse_array(parser);
    }
    else if (token->type == SBS_TOKEN_LPAREN)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node = sbs_expr_parse(parser);
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);
    }
    else
    {
        sbs_parser_warning(parser, token, "is unexpected in this context.");
        sbs_parser_sync(parser, (SbsTokenType[]) { SBS_TOKEN_LBRACE }, 1);
    }

    return node;
}

SbsExpression* sbs_expr_parse_unary(SbsParser *parser)
{
    SbsExpression *unary_node = NULL;
    SbsExprOperator op = SBS_EXPR_OP_ID;
    
    if (sbs_parser_peek(parser)->type == SBS_TOKEN_OP_NOT)
    {
        sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        op = SBS_EXPR_OP_NOT;
    }

    SbsExpression *node = NULL;
    const SbsToken *token = sbs_parser_peek(parser);
    
    if ((sbs_token_equals(token, "os")
                || sbs_token_equals(token, "arch")
                || sbs_token_equals(token, "toolchain")
                || sbs_token_equals(token, "config")
                || sbs_token_equals(token, "target")
                || sbs_token_equals(token, "env")) 
            && sbs_parser_peek_at(parser, 1)->type == SBS_TOKEN_LPAREN)
    {
        // TODO: This is here fro BC: env(...), arch(...), etc
        node = parse_inlist_expression(parser);
    }
    else
    {
        node = sbs_expr_parse_primary(parser);
    }
    return (SbsExpression*) sbs_expr_make_unary(op, node);
}
