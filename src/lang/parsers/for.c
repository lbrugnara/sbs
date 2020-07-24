#include <stddef.h>
#include "for.h"
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

    SbsVariableExpr *var_node = fl_malloc(sizeof(SbsVariableExpr));
    var_node->kind = SBS_EXPR_VARIABLE;

    SbsArrayExpr *array_node = fl_malloc(sizeof(SbsArrayExpr));
    array_node->kind = SBS_EXPR_ARRAY;
    array_node->items = fl_array_new(sizeof(SbsExpression*), 0);

    bool is_compat_env = false;
    if (sbs_token_equals(token, "os"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_node->name = fl_cstring_dup("$sbs.os");
    }
    else if (sbs_token_equals(token, "arch"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_node->name = fl_cstring_dup("$sbs.arch");
    }
    else if (sbs_token_equals(token, "toolchain"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_node->name = fl_cstring_dup("$sbs.toolchain");
    }
    else if (sbs_token_equals(token, "config"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_node->name = fl_cstring_dup("$sbs.config");
    }
    else if (sbs_token_equals(token, "target"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        var_node->name = fl_cstring_dup("$sbs.target");
    }
    else // if (sbs_token_equals(token, "env")) For compatibility, if we find a for section without a keyword, it is tied to the environment
    {
        var_node->name = fl_cstring_dup("$sbs.env");
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

    while (sbs_parser_has_input(parser))
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if ((is_compat_env && token->type == SBS_TOKEN_LBRACE) || (!is_compat_env && token->type == SBS_TOKEN_RPAREN))
            break;

        char **identifiers = parse_identifiers(parser);

        for (size_t i=0; i < fl_array_length(identifiers); i++)
        {
            // TODO: We assume strings here...
            SbsValueExpr *str = fl_malloc(sizeof(SbsValueExpr));
            str->kind = SBS_EXPR_VALUE;
            str->type = SBS_EXPR_VALUE_TYPE_STR;
            str->value.s = identifiers[i];

            array_node->items = fl_array_append(array_node->items, &str);
        }

        fl_array_free(identifiers);
    }

    if (!is_compat_env)
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);

    return sbs_expression_make_binary(SBS_EVAL_OP_IN_ARRAY, (SbsExpression*) var_node, (SbsExpression*) array_node);
}


static SbsExpression* parse_unary_expression(SbsParser *parser)
{
    SbsUnaryExpr *unary_node = fl_malloc(sizeof(SbsUnaryExpr));
    unary_node->kind = SBS_EXPR_UNARY;
    unary_node->op = SBS_EVAL_OP_ID;

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_OP_NOT)
    {
        sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        unary_node->op = SBS_EVAL_OP_NOT;
    }

    unary_node->node = parse_inlist_expression(parser);

    return (SbsExpression*) unary_node;
}

static SbsExpression* parse_and_expression(SbsParser *parser)
{
    SbsExpression *node = parse_unary_expression(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_AND)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_AND);

        SbsExpression *right = parse_unary_expression(parser);

        SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));
        binode->kind = SBS_EXPR_BINARY;
        binode->op = SBS_EVAL_OP_AND;
        binode->left = node;
        binode->right = right;

        node = (SbsExpression*) binode;
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

        SbsBinaryExpr *binode = fl_malloc(sizeof(SbsBinaryExpr));
        binode->kind = SBS_EXPR_BINARY;
        binode->op = SBS_EVAL_OP_OR;
        binode->left = node;
        binode->right = right;

        node = (SbsExpression*) binode;
    }

    return node;
}

SbsSectionFor* sbs_section_for_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_FOR);

    SbsSectionFor *for_section = fl_malloc(sizeof(SbsSectionFor));
    for_section->expr = parse_or_expression(parser);

    return for_section;
}
