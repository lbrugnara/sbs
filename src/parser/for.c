#include <stddef.h>
#include "for.h"
#include "parser.h"

static SbsNodeFor* parse_value_expression(SbsParser *parser);
static SbsNodeFor* parse_unary_expression(SbsParser *parser);
static SbsNodeFor* parse_and_expression(SbsParser *parser);
static SbsNodeFor* parse_or_expression(SbsParser *parser);

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

void sbs_section_for_free(SbsSectionFor *section)
{
    sbs_for_node_free(section->condition);
    fl_free(section);
}

static SbsNodeFor* parse_value_expression(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    if (token->type == SBS_TOKEN_LPAREN)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        SbsNodeFor *node = parse_or_expression(parser);
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);
        return node;
    }

    SbsForValueNode *node = fl_malloc(sizeof(SbsForValueNode));
    node->kind = SBS_FOR_NODE_VALUE;

    bool is_compat_env = false;
    if (sbs_token_equals(token, "os"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node->resource = SBS_FOR_RESOURCE_OS;
    }
    else if (sbs_token_equals(token, "arch"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node->resource = SBS_FOR_RESOURCE_ARCH;
    }
    else if (sbs_token_equals(token, "toolchain"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node->resource = SBS_FOR_RESOURCE_TOOLCHAIN;
    }
    else if (sbs_token_equals(token, "config"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node->resource = SBS_FOR_RESOURCE_CONFIG;
    }
    else if (sbs_token_equals(token, "target"))
    {
        sbs_parser_consume(parser, token->type);
        sbs_parser_consume(parser, SBS_TOKEN_LPAREN);
        node->resource = SBS_FOR_RESOURCE_TARGET;
    }
    else // if (sbs_token_equals(token, "env")) For compatibility, if we find a for section without a keyword, it is tied to the environment
    {
        node->resource = SBS_FOR_RESOURCE_ENV;
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

        node->variables = parse_identifiers(parser);
    }

    if (!is_compat_env)
        sbs_parser_consume(parser, SBS_TOKEN_RPAREN);

    return (SbsNodeFor*) node;
}


static SbsNodeFor* parse_unary_expression(SbsParser *parser)
{
    SbsForUnaryNode *unary_node = fl_malloc(sizeof(SbsForUnaryNode));
    unary_node->kind = SBS_FOR_NODE_UNARY;
    unary_node->op = SBS_FOR_OP_ID;

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_OP_NOT)
    {
        sbs_parser_consume(parser, SBS_TOKEN_OP_NOT);
        unary_node->op = SBS_FOR_OP_NOT;
    }

    unary_node->node = parse_value_expression(parser);

    return (SbsNodeFor*) unary_node;
}

static SbsNodeFor* parse_and_expression(SbsParser *parser)
{
    SbsNodeFor *node = parse_unary_expression(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_AND)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_AND);

        SbsNodeFor *right = parse_unary_expression(parser);

        SbsForBinaryNode *binode = fl_malloc(sizeof(SbsForBinaryNode));
        binode->kind = SBS_FOR_NODE_BINARY;
        binode->op = SBS_FOR_OP_AND;
        binode->left = node;
        binode->right = right;

        node = (SbsNodeFor*) binode;
    }

    return node;
}

static SbsNodeFor* parse_or_expression(SbsParser *parser)
{
    SbsNodeFor *node = parse_and_expression(parser);

    while (sbs_parser_has_input(parser) && sbs_parser_peek(parser)->type == SBS_TOKEN_OP_OR)
    {
        const SbsToken *or_op = sbs_parser_consume(parser, SBS_TOKEN_OP_OR);

        SbsNodeFor *right = parse_and_expression(parser);

        SbsForBinaryNode *binode = fl_malloc(sizeof(SbsForBinaryNode));
        binode->kind = SBS_FOR_NODE_BINARY;
        binode->op = SBS_FOR_OP_OR;
        binode->left = node;
        binode->right = right;

        node = (SbsNodeFor*) binode;
    }

    return node;
}

SbsSectionFor* sbs_section_for_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_FOR);

    SbsSectionFor *for_section = fl_malloc(sizeof(SbsSectionFor));
    for_section->condition = parse_or_expression(parser);

    return for_section;
}

SbsSectionFor* sbs_section_for_copy(SbsSectionFor *section)
{
    SbsSectionFor *copy = fl_malloc(sizeof(SbsSectionFor));

    copy->condition = sbs_for_node_copy(section->condition);

    return copy;
}

SbsSectionFor* sbs_section_for_extend(SbsSectionFor *child_section, SbsSectionFor *parent_section)
{
    if (!parent_section)
        return child_section;

    if (child_section == NULL)
    {
        child_section = sbs_section_for_copy(parent_section);
    }
    else
    {
        child_section->condition = sbs_for_node_make_binary(SBS_FOR_OP_AND, child_section->condition, sbs_for_node_copy(parent_section->condition));
    }

    return child_section;
}
