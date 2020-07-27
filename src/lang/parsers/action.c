#include <fllib.h>
#include "action.h"
#include "helpers.h"
#include "parser.h"
#include "for.h"
#include "command.h"

static void parse_action_body(SbsParser *parser, SbsNodeAction *current_node)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_FOR)
    { 
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_COMMAND_STRING;
            current_node->commands[size].value = sbs_parse_command_string(parser);
        }
        else if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_COMMAND_NAME;
            current_node->commands[size].value = sbs_parse_identifier(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing an action body");
        }
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_section_action_parse
 *  Parses an *action* block which supports the following elements withing its body:
 *      - A string: It is processed as a command to execute when the action is invoked
 *      - An identifier: The name of an existant action
 *      - A *for* block which is an action that can be invoked just for the specific environments it declares
 *          (accepts strings and identifiers commands too).
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsSectionAction* - The parsed *action* block
 *
 */
SbsSectionAction* sbs_section_action_parse(SbsParser *parser)
{
    // Consume 'action'
    sbs_parser_consume(parser, SBS_TOKEN_ACTION);
    
    // Consume IDENTIFIER
    SbsSectionAction *action_section = sbs_section_action_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        SbsNodeAction *action_node = sbs_section_action_add_node(action_section);
        
        if (token->type == SBS_TOKEN_FOR)
        {
            action_node->for_clause = sbs_section_for_parse(parser);

            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_action_body(parser, action_node);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
        }
        else
        {
            parse_action_body(parser, action_node);
        }
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return action_section;
}

/*
 * Function: sbs_property_actions_parse
 *  Parses an *actions* block that supports the following properties:
 *      - before: An array of strings or ids that identify actions to be run before a specific moment
 *      - after: An array of strings or ids that identify actions to be run after a specific moment
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsPropertyActions - Parsed *actions* block
 *
 */
SbsPropertyActions sbs_property_actions_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    sbs_parser_consume(parser, SBS_TOKEN_COLON);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    SbsPropertyActions actions = {0};
    const SbsToken *token = NULL;

    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"before", 6))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.before = sbs_value_command_array_parse(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"after", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.after = sbs_value_command_array_parse(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing an action block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return actions;
}
