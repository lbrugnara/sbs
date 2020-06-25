#include <fllib.h>
#include "action.h"
#include "common.h"
#include "parser.h"
#include "../common/common.h"

static void action_node_free(void *value);

static void parse_action_body(SbsParser *parser, SbsActionNode *current_node)
{
    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE && token->type != SBS_TOKEN_FOR)
    { 
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_STRING;
            current_node->commands[size].value = sbs_common_parse_command_string(parser);
        }
        else if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            size_t size = fl_array_length(current_node->commands);
            current_node->commands = fl_array_resize(current_node->commands, size + 1);
            current_node->commands[size].type = SBS_IDENTIFIER;
            current_node->commands[size].value = sbs_common_parse_identifier(parser);
        }
        else
        {
            sbs_parser_error(token, "while parsing an action body");
        }
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_action_section_parse
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
 *  SbsActionSection* - The parsed *action* block
 *
 */
SbsActionSection* sbs_action_section_parse(SbsParser *parser)
{
    SbsActionSection *action_section = fl_malloc(sizeof(SbsActionSection));

    // Consume 'action'
    sbs_parser_consume(parser, SBS_TOKEN_ACTION);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    action_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    FlVector *action_nodes = fl_vector_new_args((struct FlVectorArgs) {
        .writer = fl_container_writer,
        .element_size = sizeof(SbsActionNode)
    });

    const SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (token->type == SBS_TOKEN_FOR)
        {
            // We "reset" the current node to parse the for declaration
            SbsActionNode current_node = { 
                .commands = fl_array_new(sizeof(SbsStringOrId), 0),
                .for_envs = sbs_common_parse_for_declaration(parser)
            };

            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_action_body(parser, &current_node);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

            fl_vector_add(action_nodes, &current_node);
        }
        else
        {
            SbsActionNode current_node = { 
                .commands = fl_array_new(sizeof(SbsStringOrId), 0),
                .for_envs = NULL
            };

            parse_action_body(parser, &current_node);

            fl_vector_add(action_nodes, &current_node);
        }

       sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    action_section->nodes = fl_vector_to_array(action_nodes);
    fl_vector_free(action_nodes);

    return action_section;
}

static void action_node_free(void *value)
{
    SbsActionNode *command = (SbsActionNode*)value;

    if (command->commands)
        fl_array_free_each(command->commands, sbs_common_free_string_or_id);
    if (command->for_envs)
        fl_array_free_each(command->for_envs, sbs_common_free_string);

    // no need to free "command" as it is a struct value
}

void sbs_action_section_free(SbsActionSection *action)
{
    fl_cstring_free(action->name);

    if (action->nodes)
        fl_array_free_each(action->nodes, action_node_free);

    fl_free(action);
}

/*
 * Function: sbs_actions_node_parse
 *  Parses an *actions* block that supports the following properties:
 *      - before: An array of strings or ids that identify actions to be run before a specific moment
 *      - after: An array of strings or ids that identify actions to be run after a specific moment
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsActionsNode - Parsed *actions* block
 *
 */
SbsActionsNode sbs_actions_node_parse(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    sbs_parser_consume(parser, SBS_TOKEN_COLON);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    SbsActionsNode actions = {0};
    const SbsToken *token = NULL;

    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"before", 6))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.before = sbs_common_parse_command_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"after", 5))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            actions.after = sbs_common_parse_command_string_or_id_array(parser);
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

void sbs_actions_node_free(SbsActionsNode *actions)
{
    if (actions->before)
        fl_array_free_each(actions->before, sbs_common_free_string_or_id);

    if (actions->after)
        fl_array_free_each(actions->after, sbs_common_free_string_or_id);
}
