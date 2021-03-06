#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "environment.h"
#include "expr.h"
#include "helpers.h"
#include "parser.h"
#include "action.h"
#include "cstring.h"

void sbs_section_env_free(SbsSectionEnv *env)
{
    if (!env)
        return;

    if (env->os)
        sbs_expr_free(env->os);

    if (env->arch)
        sbs_expr_free(env->arch);

    if (env->name)
        fl_cstring_free(env->name);

    if (env->variables)
        fl_array_free_each_pointer(env->variables, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->shell_type)
        fl_cstring_free(env->shell_type);

    if (env->shell_command)
        fl_cstring_free(env->shell_command);

    if (env->shell_args)
        fl_array_free_each_pointer(env->shell_args, (FlArrayFreeElementFunc) fl_cstring_free);

    if (env->condition)
        sbs_expr_free(env->condition);

    sbs_property_actions_free(&env->actions);

    free(env);
}

SbsSectionEnv* sbs_section_env_new(const struct FlSlice *name)
{
    SbsSectionEnv *env = fl_malloc(sizeof(SbsSectionEnv));

    env->name = sbs_slice_to_cstring(name);

    return env;
}


/*
 * Function: sbs_section_env_parse
 *  Parses an *env* block which supports the following properties:
 *      - shell_args: array of strings that represents the *argv* of the shell to be used
 *      - shell_type: The type property allows 3 predefined identifiers: bash, cmd, powershell.
 *      - shell_command: The path to an executable shell
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
    // Consume 'env'
    sbs_parser_consume(parser, SBS_TOKEN_ENV);
    
    // Consume IDENTIFIER
    SbsSectionEnv *env = sbs_section_env_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        env->condition = sbs_expr_parse_for(parser);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "shell_args"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->shell_args = sbs_cstring_array_parse(parser);
            if (env->shell_args)
            {
                // argv[argc] must be NULL, so we make room for it
                // and set the last element to NULL
                size_t length = fl_array_length(env->shell_args);
                env->shell_args = fl_array_resize(env->shell_args, length + 1);
                env->shell_args[length] = NULL;
            }
        }
        else if (sbs_token_equals(token, "shell_type"))
        {
            const SbsToken *shell_token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->shell_type = sbs_parse_identifier(parser);

            if (!flm_cstring_equals(env->shell_type, "bash")
                && !flm_cstring_equals(env->shell_type, "cmd")
                && !flm_cstring_equals(env->shell_type, "powershell"))
            {
                sbs_parser_warning(parser, shell_token, "Unknown shell type");
            }
        }
        else if (sbs_token_equals(token, "shell_command"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->shell_command = sbs_cstring_parse(parser);
        }
        else if (sbs_token_equals(token, "variables"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->variables = sbs_cstring_array_parse(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            env->actions = sbs_property_actions_parse(parser);
        }
        else if (sbs_token_equals(token, "os"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->os = sbs_expr_parse(parser);
        }
        else if (sbs_token_equals(token, "arch"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            env->arch = sbs_expr_parse(parser);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return env;
}
