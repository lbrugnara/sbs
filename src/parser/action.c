#include <fllib.h>
#include "action.h"
#include "helpers.h"
#include "parser.h"
#include "../common/common.h"

static void action_node_free(SbsNodeAction *action_node);

static void parse_action_body(SbsParser *parser, SbsNodeAction *current_node)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_FOR)
    { 
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_STRING;
            current_node->commands[size].value = sbs_parse_command_string(parser);
        }
        else if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_IDENTIFIER;
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
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    SbsSectionAction *action_section = fl_malloc(sizeof(SbsSectionAction));
    action_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);
    action_section->nodes = fl_array_new(sizeof(SbsNodeAction*), 0);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        SbsNodeAction *action_node = fl_malloc(sizeof(SbsNodeAction));
        action_node->commands = fl_array_new(sizeof(SbsStringOrId), 0);

        action_section->nodes = fl_array_append(action_section->nodes, &action_node);
        
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

static void action_node_free(SbsNodeAction *action_node)
{
    if (action_node->commands)
        fl_array_free_each(action_node->commands, sbs_common_free_string_or_id);

    if (action_node->for_clause)
        sbs_section_for_free(action_node->for_clause);

    fl_free(action_node);
}

void sbs_section_action_free(SbsSectionAction *action_section)
{
    fl_cstring_free(action_section->name);

    if (action_section->nodes)
        fl_array_free_each_pointer(action_section->nodes, (FlArrayFreeElementFunc) action_node_free);

    fl_free(action_section);
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
            actions.before = sbs_parse_command_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"after", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.after = sbs_parse_command_string_or_id_array(parser);
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

void sbs_property_actions_free(SbsPropertyActions *actions)
{
    if (actions->before)
        fl_array_free_each(actions->before, sbs_common_free_string_or_id);

    if (actions->after)
        fl_array_free_each(actions->after, sbs_common_free_string_or_id);
}
