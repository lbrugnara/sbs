#include <fllib.h>
#include "action.h"
#include "helpers.h"
#include "parser.h"
#include "conditional.h"
#include "command.h"

static void parse_action_body(SbsParser *parser, SbsNodeAction *current_node)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_IF)
    { 
        SbsValueCommand *command = NULL;
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            command = fl_malloc(sizeof(SbsValueCommand));
            command->type = SBS_VALUE_COMMAND_STRING;
            command->value = sbs_value_command_string_parse(parser);
        }
        else if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            command = fl_malloc(sizeof(SbsValueCommand));
            command->type = SBS_VALUE_COMMAND_NAME;
            command->value = sbs_string_new(sbs_parse_identifier(parser), true);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing an action body");
        }

        current_node->commands = fl_array_append(current_node->commands, &command);

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
        
        if (token->type == SBS_TOKEN_IF)
        {
            action_node->condition = sbs_stmt_conditional_parse(parser);

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
            sbs_parser_error(parser, token, "while parsing an action block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return actions;
}
