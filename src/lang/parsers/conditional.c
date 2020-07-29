#include <stddef.h>
#include "conditional.h"
#include "parser.h"

static char** parse_identifiers(SbsParser *parser);
static SbsExpression* parse_inlist_expression(SbsParser *parser);
static SbsExpression* parse_unary_expression(SbsParser *parser);
static SbsExpression* parse_and_expression(SbsParser *parser);
static SbsExpression* parse_or_expression(SbsParser *parser);

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
        SbsExpression *node = parse_or_expression(parser);
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);
        return node;
    }

    const char *var_name = NULL;
    bool is_compat_env = false;

    if (sbs_token_equals(token, "os"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "sbs.os";
    }
    else if (sbs_token_equals(token, "arch"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "sbs.arch";
    }
    else if (sbs_token_equals(token, "toolchain"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "sbs.toolchain";
    }
    else if (sbs_token_equals(token, "config"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "sbs.config";
    }
    else if (sbs_token_equals(token, "target"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_name = "sbs.target";
    }
    else // if (sbs_token_equals(token, "env")) For compatibility, if we find a for section without a keyword, it is tied to the environment
    {
        var_name = "sbs.env";
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

    SbsExpression *var_node = (SbsExpression*) sbs_expression_make_variable(var_name);
    SbsArrayExpr *array_node = sbs_expression_make_array();

    while (sbs_parser_has_input(parser))
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if ((is_compat_env && token->type == SBS_TOKEN_LBRACE) || (!is_compat_env && token->type == SBS_TOKEN_RPAREN))
            break;

        char **identifiers = parse_identifiers(parser);

        for (size_t i=0; i < fl_array_length(identifiers); i++)
        {
            // TODO: We assume strings here...
            SbsValueExpr *str = sbs_expression_make_value(SBS_EXPR_VALUE_TYPE_STR);
            str->value.s = identifiers[i];

            sbs_expression_array_add_item(array_node, (SbsExpression*) str);
        }

        fl_array_free(identifiers);
    }

    if (!is_compat_env)
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);

    return (SbsExpression*) sbs_expression_make_binary(SBS_EVAL_OP_IN, var_node, (SbsExpression*) array_node);
}

static SbsExpression* parse_identifier_expression(SbsParser *parser)
{
    const SbsToken *id = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    
    // TODO: We assume strings here...
    SbsValueExpr *str = sbs_expression_make_value(SBS_EXPR_VALUE_TYPE_STR);
    str->value.s = sbs_token_to_str(id);

    return (SbsExpression*) str;
}

static SbsExpression* parse_variable_expression(SbsParser *parser)
{
    const SbsToken *var_token = sbs_parser_consume(parser, SBS_TOKEN_VARIABLE);

    // TODO: Should we use slice to avoid the dup string?
    char *var_name = sbs_token_to_str(var_token);
    SbsExpression *var_node = (SbsExpression*) sbs_expression_make_variable(var_name);
    fl_cstring_free(var_name);

    return var_node;
}

static SbsExpression* parse_array_expression(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    SbsArrayExpr *array_node = sbs_expression_make_array();

    while (sbs_parser_next_is_not(parser, SBS_TOKEN_RBRACKET))
    {
        SbsExpression *item = parse_or_expression(parser);

        if (item != NULL)
        {
            sbs_expression_array_add_item(array_node, (SbsExpression*) item);
        }

        if (sbs_parser_next_is_not(parser, SBS_TOKEN_RBRACKET))
            sbs_parser_consume(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return (SbsExpression*) array_node;
}

static SbsExpression* parse_primary_expression(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    SbsExpression *node = NULL;

    if (token->type == SBS_TOKEN_VARIABLE)
    {
        node = parse_variable_expression(parser);
    }
    else if (token->type == SBS_TOKEN_IDENTIFIER)
    {
        node = parse_identifier_expression(parser);
    }
    else if (token->type == SBS_TOKEN_LBRACKET)
    {
        node = parse_array_expression(parser);
    }
    else if (token->type == SBS_TOKEN_LPAREN)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node = parse_or_expression(parser);
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);
    }
    else
    {
        sbs_parser_warning(parser, token, "is unexpected in this context.");
        sbs_parser_sync(parser, (SbsTokenType[]) { SBS_TOKEN_LBRACE }, 1);
    }

    return node;
}

static SbsExpression* parse_unary_expression(SbsParser *parser)
{
    SbsExpression *unary_node = NULL;
    SbsEvalOperatorKind op = SBS_EVAL_OP_ID;
    
    if (sbs_parser_peek(parser)->type == SBS_TOKEN_OP_NOT)
    {
        sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        op = SBS_EVAL_OP_NOT;
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
        node = parse_primary_expression(parser);
    }
    return (SbsExpression*) sbs_expression_make_unary(op, node);
}

static SbsExpression* parse_comparison_expression(SbsParser *parser)
{
    SbsExpression *node = parse_unary_expression(parser);

    while (sbs_parser_has_input(parser))
    {
        bool is_not = false;
        SbsEvalOperatorKind operator = SBS_EVAL_OP_UNK;

        if (sbs_parser_next_is(parser, SBS_TOKEN_OP_NOT))
        {
            is_not = true;
            sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        }

        if (sbs_parser_next_is(parser, SBS_TOKEN_OP_IN))
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_IN);
            operator = SBS_EVAL_OP_IN;
        }
        else
        {
            break;
        }

        SbsExpression *right = parse_unary_expression(parser);

        node = (SbsExpression*) sbs_expression_make_binary(operator, node, right);

        if (is_not)
            node = (SbsExpression*) sbs_expression_make_unary(SBS_EVAL_OP_NOT, node);
    }

    return node;
}

static SbsExpression* parse_equality_expression(SbsParser *parser)
{
    SbsExpression *node = parse_comparison_expression(parser);

    while (sbs_parser_has_input(parser))
    {
        SbsEvalOperatorKind operator = SBS_EVAL_OP_UNK;
        const SbsToken *operator_token = sbs_parser_peek(parser);

        if (operator_token->type == SBS_TOKEN_OP_EQ)
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_EQ);
            operator = SBS_EVAL_OP_EQ;
        }
        else if (operator_token->type == SBS_TOKEN_OP_NEQ)
        {
            sbs_parser_consume(parser, SBS_TOKEN_OP_NEQ);
            operator = SBS_EVAL_OP_NEQ;
        }
        else
        {
            break;
        }

        SbsExpression *right = parse_comparison_expression(parser);

        node = (SbsExpression*) sbs_expression_make_binary(operator, node, right);
    }

    return node;
}

static SbsExpression* parse_and_expression(SbsParser *parser)
{
    SbsExpression *node = parse_equality_expression(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_AND)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_AND);

        SbsExpression *right = parse_equality_expression(parser);

        node = (SbsExpression*) sbs_expression_make_binary(SBS_EVAL_OP_AND, node, right);
    }

    return node;
}

static SbsExpression* parse_or_expression(SbsParser *parser)
{
    SbsExpression *node = parse_and_expression(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_OR)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_OR);

        SbsExpression *right = parse_and_expression(parser);

        node = (SbsExpression*) sbs_expression_make_binary(SBS_EVAL_OP_OR, node, right);
    }

    return node;
}

SbsStmtConditional* sbs_stmt_conditional_parse(SbsParser *parser)
{
    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
    {
        sbs_parser_consume(parser, SBS_TOKEN_FOR);
    }
    else if (sbs_parser_peek(parser)->type == SBS_TOKEN_IF)
    {
        sbs_parser_consume(parser, SBS_TOKEN_IF);
    }
    else
    {
        return NULL;
    }

    SbsStmtConditional *conditional = sbs_stmt_conditional_new();
    conditional->expr = parse_or_expression(parser);

    return conditional;
}
