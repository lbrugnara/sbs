#include "environment.h"
#include "common.h"
#include "parser.h"
#include "../common/common.h"

void sbs_env_section_free(SbsEnvSection *env)
{
    if (!env)
        return;

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each(env->variables, sbs_common_free_string);

    if (env->type)
        fl_cstring_free(env->type);

    if (env->terminal)
        fl_cstring_free(env->terminal);

    if (env->args)
        fl_array_free_each(env->args, sbs_common_free_string);

    sbs_actions_node_free(&env->actions);

    free(env);
}

/*
 * Function: sbs_env_section_parse
 *  Parses an *env* block which supports the following properties:
 *      - args: array of strings that represents the *argv* of the shell to be used
 *      - type: The type property allows 3 predefined identifiers: bash, cmd, powershell.
 *      - terminal: The path to an executable shell
 *      - variables: Array of strings with the form of "key=value"
 *      - actions: <SbsActionsNode> object with commands to be run before and after the build process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  static SbsEnvSection* - The parsed *env* block
 *
 */
SbsEnvSection* sbs_env_section_parse(SbsParser *parser)
{
    SbsEnvSection *env = fl_malloc(sizeof(SbsEnvSection));

    // Consume 'env'
    sbs_parser_consume(parser, SBS_TOKEN_ENV);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    env->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"args", 4))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->args = sbs_common_parse_string_array(parser);
            if (env->args)
            {
                // argv[argc] must be NULL, so we make room for it
                // and set the last element to NULL
                size_t length = fl_array_length(env->args);
                env->args = fl_array_resize(env->args, length + 1);
                env->args[length] = NULL;
            }
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"type", 4))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->type = sbs_common_parse_identifier(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"terminal", 8))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->terminal = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"variables", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->variables = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            env->actions = sbs_actions_node_parse(parser);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return env;
}
