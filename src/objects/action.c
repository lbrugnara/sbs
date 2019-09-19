#include <fllib.h>
#include "action.h"
#include "common.h"
#include "parser.h"

void free_action_command(FlByte *value)
{
    struct SbsActionCommand *command = (struct SbsActionCommand*)value;

    if (command->commands)
        fl_array_delete_each(command->commands, sbs_common_free_string);
    if (command->for_envs)
        fl_array_delete_each(command->for_envs, sbs_common_free_string);

    // no need to free "command" as it is a struct value
}

void sbs_action_free(struct SbsAction *action)
{
    fl_cstring_delete(action->name);

    if (action->commands)
        fl_array_delete_each(action->commands, free_action_command);

    fl_free(action);
}

static void free_map_entry(void *value)
{
    sbs_action_free((struct SbsAction*)value);
}

void sbs_action_map_init(FlHashtable *actions)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_map_entry
    };
    
    *actions = fl_hashtable_new_args(new_args);
}

/*
 * Function: sbs_action_parse
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
 *  struct SbsAction* - The parsed *action* block
 *
 */
struct SbsAction* sbs_action_parse(struct SbsParser *parser)
{
    struct SbsAction *action = fl_malloc(sizeof(struct SbsAction));

    // Consume 'action'
    sbs_parser_consume(parser, SBS_TOKEN_ACTION);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    action->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    /*if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
        action->extends = sbs_common_parse_extends_declaration(parser);*/

    // action_declaration -> 'action' IDENTIFIER '{' action_command* '}'
    // action_command -> command | for_command
    // command -> STRING
    // for_command -> for_declaration '{' command* '}'
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    FlVector commands = fl_vector_new_args((struct FlVectorArgs) {
        .writer = fl_container_writer,
        .element_size = sizeof(struct SbsActionCommand)
    });

    const struct SbsToken *token = NULL;
    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            struct SbsActionCommand command = {
                .commands =  fl_array_new(sizeof(char*), 1),
                .for_envs = NULL
            };
            
            command.commands[0] = sbs_common_parse_command_string(parser);

            fl_vector_add(commands, &command);
        }
        if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            struct SbsActionCommand command = {
                .commands =  fl_array_new(sizeof(char*), 1),
                .for_envs = NULL
            };
            
            command.commands[0] = sbs_common_parse_identifier(parser);

            fl_vector_add(commands, &command);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            struct SbsActionCommand command = {
                .for_envs = sbs_common_parse_for_declaration(parser)
            };

            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            
            size_t commandq = 0;
            size_t offset = 0;
            while ((token = sbs_parser_peek_at(parser, offset++)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (token->type == SBS_TOKEN_COMMAND_STRING || token->type == SBS_TOKEN_IDENTIFIER)
                    commandq++;
            }

            command.commands = fl_array_new(sizeof(char*), commandq);
            offset = 0;
            while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (token->type == SBS_TOKEN_COMMAND_STRING)
                {
                    command.commands[offset++] = sbs_common_parse_command_string(parser);
                }
                else if (token->type == SBS_TOKEN_IDENTIFIER)
                {
                    command.commands[offset++] = sbs_common_parse_identifier(parser);
                }
                sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
            }

            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

            fl_vector_add(commands, &command);
        }

       sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    action->commands = fl_vector_to_array(commands);
    fl_vector_delete(commands);

    return action;
}
/*
 * Function: sbs_actions_parse
 *  Parses an *actions* block that supports the following properties:
 *      - before: An array of strings or ids that identify actions to be run before a specific moment
 *      - after: An array of strings or ids that identify actions to be run after a specific moment
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsActions - Parsed *actions* block
 *
 */
struct SbsActions sbs_actions_parse(struct SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
    sbs_parser_consume(parser, SBS_TOKEN_COLON);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    struct SbsActions actions = {0};
    const struct SbsToken *token = NULL;

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
            sbs_parser_error(token);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return actions;
}
