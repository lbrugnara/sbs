#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "action.h"
#include "command.h"
#include "../utils.h"
#include "parser.h"
#include "expression.h"
#include "parsers/expression.h"
#include "command.h"

static void action_node_free(SbsNodeAction *action_node)
{
    if (action_node->commands)
        fl_array_free_each_pointer(action_node->commands, (FlArrayFreeElementFunc) sbs_command_free);

    if (action_node->condition)
        sbs_expression_free(action_node->condition);

    fl_free(action_node);
}

void sbs_section_action_free(SbsSectionAction *action_section)
{
    fl_cstring_free(action_section->name);

    if (action_section->nodes)
        fl_array_free_each_pointer(action_section->nodes, (FlArrayFreeElementFunc) action_node_free);

    fl_free(action_section);
}

void sbs_property_actions_free(SbsPropertyActions *actions)
{
    if (actions->before)
        fl_array_free_each_pointer(actions->before, (FlArrayFreeElementFunc) sbs_command_free);

    if (actions->after)
        fl_array_free_each_pointer(actions->after, (FlArrayFreeElementFunc) sbs_command_free);
}

SbsSectionAction* sbs_section_action_new(const struct FlSlice *name)
{
    SbsSectionAction *action_section = fl_malloc(sizeof(SbsSectionAction));

    action_section->name = sbs_slice_to_cstring(name);
    action_section->nodes = fl_array_new(sizeof(SbsNodeAction*), 0);

    return action_section;
}

SbsNodeAction* sbs_section_action_add_node(SbsSectionAction *action_section)
{
    SbsNodeAction *action_node = fl_malloc(sizeof(SbsNodeAction));
    action_node->commands = fl_array_new(sizeof(SbsCommand*), 0);

    action_section->nodes = fl_array_append(action_section->nodes, &action_node);

    return action_node;
}


static void parse_action_body(SbsParser *parser, SbsNodeAction *current_node)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_IF)
    { 
        SbsCommand *command = sbs_command_parse(parser);

        if (command == NULL)
            continue;

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
            action_node->condition = sbs_statement_if_parse(parser);

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
            actions.before = sbs_command_array_parse(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"after", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.after = sbs_command_array_parse(parser);
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
