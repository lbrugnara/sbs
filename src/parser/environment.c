#include "environment.h"
#include "helpers.h"
#include "parser.h"
#include "variable.h"
#include "../common/common.h"

void sbs_section_env_free(SbsSectionEnv *env)
{
    if (!env)
        return;

    if (env->os)
        sbs_common_free_variable(env->os);

    if (env->arch)
        fl_array_free_each_pointer(env->arch, (FlArrayFreeElementFunc) sbs_common_free_variable);

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each_pointer(env->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->type)
        fl_cstring_free(env->type);

    if (env->terminal)
        fl_cstring_free(env->terminal);

    if (env->args)
        fl_array_free_each_pointer(env->args, (FlArrayFreeElementFunc) fl_cstring_free);

    sbs_property_actions_free(&env->actions);

    free(env);
}

/*
 * Function: sbs_section_env_parse
 *  Parses an *env* block which supports the following properties:
 *      - args: array of strings that represents the *argv* of the shell to be used
 *      - type: The type property allows 3 predefined identifiers: bash, cmd, powershell.
 *      - terminal: The path to an executable shell
 *      - variables: Array of strings with the form of "key=value"
 *      - actions: <SbsPropertyActions> object with commands to be run before and after the build process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  static SbsSectionEnv* - The parsed *env* block
 *
 */
SbsSectionEnv* sbs_section_env_parse(SbsParser *parser)
{
    SbsSectionEnv *env = fl_malloc(sizeof(SbsSectionEnv));

    // Consume 'env'
    sbs_parser_consume(parser, SBS_TOKEN_ENV);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    env->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "args"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->args = sbs_parse_string_array(parser);
            if (env->args)
            {
                // argv[argc] must be NULL, so we make room for it
                // and set the last element to NULL
                size_t length = fl_array_length(env->args);
                env->args = fl_array_resize(env->args, length + 1);
                env->args[length] = NULL;
            }
        }
        else if (sbs_token_equals(token, "type"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->type = sbs_parse_identifier(parser);
        }
        else if (sbs_token_equals(token, "terminal"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->terminal = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "variables"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->variables = sbs_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            env->actions = sbs_property_actions_parse(parser);
        }
        else if (sbs_token_equals(token, "os"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->os = sbs_parse_variable(parser);
        }
        else if (sbs_token_equals(token, "arch"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);

            if (sbs_parser_peek(parser)->type == SBS_TOKEN_LBRACKET)
            {
                env->arch = sbs_parse_variable_array(parser);
            }
            else
            {
                SbsVariable *variable = sbs_parse_variable(parser);
                env->arch = fl_array_new(sizeof(SbsVariable*), 1);
                env->arch[0] = variable;
            }
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return env;
}
