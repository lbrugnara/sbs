#include <fllib.h>

#include "types.h"
#include "file.h"
#include "lexer.h"
#include "parser.h"

#define DEFAULT_COUNT_TARGETS 20

struct Parser {
    const struct SbsToken *tokens;
    unsigned int index;
    size_t length;
};

#define exit_with_unexpected_token(token)   \
    flm_vexit(ERR_FATAL, "Unexpected token %s %.*s in line %ld, column %ld",   \
        token_type_string[token->type], \
        token->value.length, \
        token->value.sequence,  \
        token->line,    \
        token->col)

/* -------------------------------------------------------------
* Cleaners
* -------------------------------------------------------------
*/

void free_string(FlByte *obj)
{
    if (!obj)
        return;

    char *str = *(char**)obj;
    fl_cstring_delete(str);
}

void free_string_or_id(FlByte *obj)
{
    if (!obj)
        return;

    struct SbsStringOrId *str = (struct SbsStringOrId*)obj;

    if (str->value)
        fl_cstring_delete(str->value);
}

void free_env(void *value)
{
    struct SbsEnv *env = (struct SbsEnv*)value;

    if (!env)
        return;

    if (env->name)
        fl_cstring_delete(env->name);

    if (env->variables)
        fl_array_delete_each(env->variables, free_string);

    if (env->type)
        fl_cstring_delete(env->type);

    if (env->terminal)
        fl_cstring_delete(env->terminal);

    if (env->args)
        fl_array_delete_each(env->args, free_string);

    if (env->actions.before)
        fl_array_delete_each(env->actions.before, free_string_or_id);

    if (env->actions.after)
        fl_array_delete_each(env->actions.after, free_string_or_id);

    free(env);
}

void free_toolchain(void *value)
{
    struct SbsToolchain *toolchain = (struct SbsToolchain*)value;
    
    fl_cstring_delete(toolchain->name);
    
    if (toolchain->extends)
        fl_array_delete_each(toolchain->extends, free_string);

    if (toolchain->for_envs)
        fl_array_delete_each(toolchain->for_envs, free_string);

    if (toolchain->compiler)
        fl_cstring_delete(toolchain->compiler);

    if (toolchain->archiver)
        fl_cstring_delete(toolchain->archiver);

    if (toolchain->linker)
        fl_cstring_delete(toolchain->linker);
    
    fl_free(toolchain);
}

void free_target(void *value)
{
    struct SbsTarget *target = (struct SbsTarget*)value;
    
    fl_cstring_delete(target->name);

    if (target->actions.before)
        fl_array_delete_each(target->actions.before, free_string_or_id);

    if (target->actions.after)
        fl_array_delete_each(target->actions.after, free_string_or_id);

    if (target->output_dir)
        fl_cstring_delete(target->output_dir);


    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            struct SbsTargetCompile *compile = (struct SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_delete_each(compile->sources, free_string);

            if (compile->include_flag)
                fl_cstring_delete(compile->include_flag);

            if (compile->includes)
                fl_array_delete_each(compile->includes, free_string);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            struct SbsTargetArchive *archive = (struct SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_delete_each(archive->objects, free_string_or_id);

            if (archive->output_name)
                fl_cstring_delete(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            struct SbsTargetShared *shared = (struct SbsTargetShared*)target;

            if (shared->objects)
                fl_array_delete_each(shared->objects, free_string_or_id);

            if (shared->output_name)
                fl_cstring_delete(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            struct SbsTargetExecutable *executable = (struct SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_delete_each(executable->objects, free_string_or_id);

            if (executable->output_name)
                fl_cstring_delete(executable->output_name);

            break;
        }
    }

    fl_free(target);
}

void free_configuration(void *value)
{
    struct SbsConfiguration *config = (struct SbsConfiguration*)value;
    
    fl_cstring_delete(config->name);

    if (config->extends)
        fl_array_delete_each(config->extends, free_string);

    if (config->for_toolchains)
        fl_array_delete_each(config->for_toolchains, free_string);


    if (config->compile.flags)
        fl_array_delete_each(config->compile.flags, free_string);

    if (config->compile.defines)
        fl_array_delete_each(config->compile.defines, free_string);

    if (config->compile.extension)
        fl_cstring_delete(config->compile.extension);


    if (config->archive.flags)
        fl_array_delete_each(config->archive.flags, free_string);

    if (config->archive.extension)
        fl_cstring_delete(config->archive.extension);


    if (config->shared.flags)
        fl_array_delete_each(config->shared.flags, free_string);
    
    if (config->shared.extension)
        fl_cstring_delete(config->shared.extension);


    if (config->executable.flags)
        fl_array_delete_each(config->executable.flags, free_string);

    if (config->executable.extension)
        fl_cstring_delete(config->executable.extension);


    fl_free(config);
}

void free_action_command(FlByte *value)
{
    struct SbsActionCommand *command = (struct SbsActionCommand*)value;

    if (command->commands)
        fl_array_delete_each(command->commands, free_string);
    if (command->for_envs)
        fl_array_delete_each(command->for_envs, free_string);

    // no need to free "command" as it is a struct value
}

void free_action(void *value)
{
    struct SbsAction *action = (struct SbsAction*)value;

    fl_cstring_delete(action->name);

    if (action->commands)
        fl_array_delete_each(action->commands, free_action_command);

    fl_free(action);
}

void free_preset(void *value)
{
    struct SbsPreset *preset = (struct SbsPreset*)value;

    fl_cstring_delete(preset->name);
    fl_cstring_delete(preset->env);
    fl_cstring_delete(preset->toolchain);
    fl_cstring_delete(preset->config);
    fl_cstring_delete(preset->target);

    if (preset->actions.before)
        fl_array_delete_each(preset->actions.before, free_string_or_id);

    if (preset->actions.after)
        fl_array_delete_each(preset->actions.after, free_string_or_id);

    fl_free(preset);
}

/* -------------------------------------------------------------
* Parsing helper functions
* -------------------------------------------------------------
*/
static inline bool has_input(struct Parser *parser)
{
    return parser->index < parser->length;
}

static inline const struct SbsToken* peek(struct Parser *parser)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to peek\n");

    return parser->tokens + parser->index;
}

static inline const struct SbsToken* peek_at(struct Parser *parser, size_t offset)
{
    if (parser->index + offset >= parser->length)
        flm_vexit(ERR_FATAL, "There's no input to peek at offset %zu\n", offset);

    return parser->tokens + parser->index + offset;
}

static inline struct FlSlice peek_many(struct Parser *parser, size_t n)
{
    if (parser->index + n >= parser->length)
        return (struct FlSlice){ .sequence = NULL };

    return fl_slice_new((FlByte*)parser->tokens, sizeof(*parser->tokens), parser->index, n);
}

static inline const struct SbsToken* consume(struct Parser *parser, enum SbsTokenType type)
{
    if (parser->index >= parser->length)
        flm_exit(ERR_FATAL, "There's no input to consume\n");

    const struct SbsToken *token = parser->tokens + parser->index++;

    if (token->type != type)
        flm_vexit(ERR_FATAL, "Unexpected token %.*s\n", token->value.length, token->value.sequence);

    return token;
}

static inline void consume_if(struct Parser *parser, enum SbsTokenType type)
{
    if (!has_input(parser))
        return;

    if (peek(parser)->type == type)
        consume(parser, type);
}


/* -------------------------------------------------------------
* Parsing functions
* -------------------------------------------------------------
*/
char* parse_string(struct Parser *parser)
{
    const struct SbsToken *token = consume(parser, SBS_TOKEN_STRING);
    
    char *string = fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
    if (fl_cstring_contains(string, "\\\""))
    {
        char *tmp = fl_cstring_replace(string, "\\\"", "\"");
        if (!tmp)
            return NULL;
        fl_cstring_delete(string);
        string = tmp;
    }

    return string;
}

char* parse_command_string(struct Parser *parser)
{
    const struct SbsToken *token = consume(parser, SBS_TOKEN_COMMAND_STRING);
    
    char *string = fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
    if (fl_cstring_contains(string, "\\`"))
    {
        char *tmp = fl_cstring_replace(string, "\\`", "`");
        if (!tmp)
            return NULL;
        fl_cstring_delete(string);
        string = tmp;
    }

    return string;
}

char* parse_identifier(struct Parser *parser)
{
    const struct SbsToken *token = consume(parser, SBS_TOKEN_IDENTIFIER);
    
    return fl_cstring_dup_n((const char*)token->value.sequence, token->value.length);
}

struct SbsStringOrId* parse_command_string_or_id_array(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many elements
    size_t nelements = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        nelements++;
    }

    struct SbsStringOrId *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(struct SbsStringOrId), nelements);
        size_t index = 0;

        while (peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const struct SbsToken *element = peek(parser);

            if (element->type == SBS_TOKEN_COMMAND_STRING)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_STRING, 
                    .value = parse_command_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_IDENTIFIER,
                    .value = parse_identifier(parser)
                };
            }
            else
            {
                exit_with_unexpected_token(element);
            }

            consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}

struct SbsStringOrId* parse_string_or_id_array(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many elements
    size_t nelements = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        nelements++;
    }

    struct SbsStringOrId *elements = NULL;

    if (nelements > 0)
    {
        
        // Parse the elements
        elements = fl_array_new(sizeof(struct SbsStringOrId), nelements);
        size_t index = 0;

        while (peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            const struct SbsToken *element = peek(parser);

            if (element->type == SBS_TOKEN_STRING)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_STRING, 
                    .value = parse_string(parser) 
                };
            }
            else if (element->type == SBS_TOKEN_IDENTIFIER)
            {
                elements[index++] = (struct SbsStringOrId) {
                    .type = SBS_IDENTIFIER,
                    .value = parse_identifier(parser)
                };
            }
            else
            {
                exit_with_unexpected_token(element);
            }

            consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    consume(parser, SBS_TOKEN_RBRACKET);

    return elements;
}

char** parse_string_array(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_LBRACKET);

    // Track how many strings
    size_t stringsCount = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = peek_at(parser, i))->type != SBS_TOKEN_RBRACKET; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        stringsCount++;
    }

    char **strings = NULL;

    if (stringsCount != 0)
    {
        // Parse the strings
        strings = fl_array_new(sizeof(char*), stringsCount);
        size_t index = 0;

        while (peek(parser)->type != SBS_TOKEN_RBRACKET)
        {
            strings[index++] = parse_string(parser);

            consume_if(parser, SBS_TOKEN_COMMA);
        }
    }

    consume(parser, SBS_TOKEN_RBRACKET);

    return strings;
}

// extends_declaration -> 'extends' IDENTIFIER ( ',' IDENTIFIER )*
char** parse_extends_declaration(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_EXTENDS);

    // Track how many identifiers
    size_t identifiersCount = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = peek_at(parser, i))->type == SBS_TOKEN_COMMA || tmp->type == SBS_TOKEN_IDENTIFIER; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        identifiersCount++;
    }
    
    // Parse the identifiers
    char **identifiers = fl_array_new(sizeof(char*), identifiersCount);
    size_t index = 0;

    while (identifiersCount--)
    {
        const struct SbsToken *id = consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*)id->value.sequence, id->value.length);

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}

// for_declaration -> 'for' IDENTIFIER ( ',' IDENTIFIER )*
char** parse_for_declaration(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_FOR);

    // Track how many identifiers
    size_t identifiersCount = 0;
    const struct SbsToken *tmp;

    for (size_t i=0; (tmp = peek_at(parser, i))->type == SBS_TOKEN_COMMA || tmp->type == SBS_TOKEN_IDENTIFIER; i++)
    {
        if (tmp->type == SBS_TOKEN_COMMA)
            continue;
        identifiersCount++;
    }
    
    // Parse the identifiers
    char **identifiers = fl_array_new(sizeof(char*), identifiersCount);
    size_t index = 0;

    while (identifiersCount--)
    {
        const struct SbsToken *id = consume(parser, SBS_TOKEN_IDENTIFIER);
        identifiers[index++] = fl_cstring_dup_n((const char*)id->value.sequence, id->value.length);

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    return identifiers;
}

struct SbsActions parse_actions(struct Parser *parser)
{
    consume(parser, SBS_TOKEN_IDENTIFIER);
    consume(parser, SBS_TOKEN_COLON);
    consume(parser, SBS_TOKEN_LBRACE);

    struct SbsActions actions = {0};
    const struct SbsToken *token = NULL;

    while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"before", 6))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            actions.before = parse_command_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"after", 5))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            actions.after = parse_command_string_or_id_array(parser);
        }
        else
        {
            exit_with_unexpected_token(token);
        }

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return actions;
}

// action_declaration -> 'action' IDENTIFIER ( 'extends' IDENTIFIER )? '{' action_body? '}'
struct SbsAction* parse_action_declaration(struct Parser *parser)
{
    struct SbsAction *action = fl_malloc(sizeof(struct SbsAction));

    // Consume 'action'
    consume(parser, SBS_TOKEN_ACTION);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    action->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    /*if (peek(parser)->type == SBS_TOKEN_EXTENDS)
        action->extends = parse_extends_declaration(parser);*/

    // action_declaration -> 'action' IDENTIFIER '{' action_command* '}'
    // action_command -> command | for_command
    // command -> STRING
    // for_command -> for_declaration '{' command* '}'
    consume(parser, SBS_TOKEN_LBRACE);

    FlVector commands = fl_vector_new_args((struct FlVectorArgs) {
        .writer = fl_container_writer,
        .element_size = sizeof(struct SbsActionCommand)
    });

    const struct SbsToken *token = NULL;
    while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        if (token->type == SBS_TOKEN_COMMAND_STRING)
        {
            struct SbsActionCommand command = {
                .commands =  fl_array_new(sizeof(char*), 1),
                .for_envs = NULL
            };
            
            command.commands[0] = parse_command_string(parser);

            fl_vector_add(commands, &command);
        }
        if (token->type == SBS_TOKEN_IDENTIFIER)
        {
            struct SbsActionCommand command = {
                .commands =  fl_array_new(sizeof(char*), 1),
                .for_envs = NULL
            };
            
            command.commands[0] = parse_identifier(parser);

            fl_vector_add(commands, &command);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            struct SbsActionCommand command = {
                .for_envs = parse_for_declaration(parser)
            };

            consume(parser, SBS_TOKEN_LBRACE);
            
            size_t commandq = 0;
            size_t offset = 0;
            while ((token = peek_at(parser, offset++)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (token->type == SBS_TOKEN_COMMAND_STRING || token->type == SBS_TOKEN_IDENTIFIER)
                    commandq++;
            }

            command.commands = fl_array_new(sizeof(char*), commandq);
            offset = 0;
            while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (token->type == SBS_TOKEN_COMMAND_STRING)
                {
                    command.commands[offset++] = parse_command_string(parser);
                }
                else if (token->type == SBS_TOKEN_IDENTIFIER)
                {
                    command.commands[offset++] = parse_identifier(parser);
                }
                consume_if(parser, SBS_TOKEN_COMMA);
            }

            consume(parser, SBS_TOKEN_RBRACE);

            fl_vector_add(commands, &command);
        }

       consume_if(parser, SBS_TOKEN_COMMA);
    }

    consume(parser, SBS_TOKEN_RBRACE);

    action->commands = fl_vector_to_array(commands);
    fl_vector_delete(commands);

    return action;
}

struct SbsPreset* parse_preset_declaration(struct Parser *parser)
{
    struct SbsPreset *preset = fl_malloc(sizeof(struct SbsPreset));

    // Consume the 'preset' token
    consume(parser, SBS_TOKEN_PRESET);

    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    preset->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    const struct SbsToken *token = NULL;

    consume(parser, SBS_TOKEN_LBRACE);

    while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = peek(parser);

        if (token->type == SBS_TOKEN_ENV)
        {
            consume(parser, SBS_TOKEN_ENV);
            consume(parser, SBS_TOKEN_COLON);
            preset->env = parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_TOOLCHAIN)
        {
            consume(parser, SBS_TOKEN_TOOLCHAIN);
            consume(parser, SBS_TOKEN_COLON);
            preset->toolchain = parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_CONFIG)
        {
            consume(parser, SBS_TOKEN_CONFIG);
            consume(parser, SBS_TOKEN_COLON);
            preset->config = parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_TARGET)
        {
            consume(parser, SBS_TOKEN_TARGET);
            consume(parser, SBS_TOKEN_COLON);
            preset->target = parse_identifier(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            preset->actions = parse_actions(parser);
        }

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    consume_if(parser, SBS_TOKEN_RBRACE);

    return preset;
}

struct SbsTarget* parse_target_compile(struct Parser *parser)
{
    struct SbsTargetCompile *target = fl_malloc(sizeof(struct SbsTargetCompile));
    target->base.type = SBS_TARGET_COMPILE;

    // Consume the 'compile' token
    consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = peek(parser);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"include_flag", 12))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->include_flag = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"includes", 8))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->includes = parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"sources", 7))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->sources = parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = parse_actions(parser);
        }

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

struct SbsTarget* parse_target_archive(struct Parser *parser)
{
    struct SbsTargetArchive *target = fl_malloc(sizeof(struct SbsTargetArchive));
    target->base.type = SBS_TARGET_ARCHIVE;

    // Consume 'target'
    consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = peek(parser);
        //const struct SbsToken *token = consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->objects = parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->output_name = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = parse_actions(parser);
        }
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

struct SbsTarget* parse_target_shared(struct Parser *parser)
{
    struct SbsTargetShared *target = fl_malloc(sizeof(struct SbsTargetShared));
    target->base.type = SBS_TARGET_SHARED;

    // Consume 'target'
    consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = peek(parser);
        //const struct SbsToken *token = consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->objects = parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->output_name = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = parse_actions(parser);
        }
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

struct SbsTarget* parse_target_executable(struct Parser *parser)
{
    struct SbsTargetExecutable *target = fl_malloc(sizeof(struct SbsTargetExecutable));
    target->base.type = SBS_TARGET_EXECUTABLE;

    // Consume 'target'
    consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = peek(parser);
        //const struct SbsToken *token = consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->objects = parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->output_name = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = parse_actions(parser);
        }
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

// target_declaration -> 'target' IDENTIFIER '{' target_body '}'
struct SbsTarget* parse_target_declaration(struct Parser *parser)
{
    const struct SbsToken *token = peek(parser);

    if (token == NULL)
        return NULL;

    switch (token->type)
    {
        case SBS_TOKEN_COMPILE:
            return parse_target_compile(parser);
        case SBS_TOKEN_ARCHIVE:
            return parse_target_archive(parser);
        case SBS_TOKEN_SHARED:
            return parse_target_shared(parser);
        case SBS_TOKEN_EXECUTABLE:
            return parse_target_executable(parser);
        default:
            exit_with_unexpected_token(token);
    }

    return NULL;
}

struct SbsToolchain* parse_toolchain_declaration(struct Parser *parser)
{
    struct SbsToolchain *toolchain = fl_malloc(sizeof(struct SbsToolchain));

    // Consume 'toolchain'
    consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    toolchain->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (peek(parser)->type == SBS_TOKEN_EXTENDS)
        toolchain->extends = parse_extends_declaration(parser);

    if (peek(parser)->type == SBS_TOKEN_FOR)
        toolchain->for_envs = parse_for_declaration(parser);

    // toolchain_body -> 'compiler' ':' STRING ','?
    //             | 'archiver' ':' STRING ','?
    //             | 'linker' ':' STRING ','?
    //             ;

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *property = consume(parser, SBS_TOKEN_IDENTIFIER);
        consume(parser, SBS_TOKEN_COLON);

        // So far, all properties are string 
        const struct SbsToken *value = peek(parser);

        if (fl_slice_equals_sequence(&property->value, (FlByte*)"compiler", 8))
        {
            toolchain->compiler = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&property->value, (FlByte*)"archiver", 8))
        {
            toolchain->archiver = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&property->value, (FlByte*)"linker", 6))
        {
            toolchain->linker = parse_string(parser);
        }
        else
        {
            exit_with_unexpected_token(value);
        }
        
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return toolchain;
}

// config_declaration -> 'config' IDENTIFIER ( 'extends' IDENTIFIER )? '{' config_body? '}'
struct SbsConfiguration* parse_configuration_declaration(struct Parser *parser)
{
    struct SbsConfiguration *configuration = fl_malloc(sizeof(struct SbsConfiguration));

    // Consume 'configuration'
    consume(parser, SBS_TOKEN_CONFIG);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    configuration->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (peek(parser)->type == SBS_TOKEN_EXTENDS)
        configuration->extends = parse_extends_declaration(parser);

    if (peek(parser)->type == SBS_TOKEN_FOR)
        configuration->for_toolchains = parse_for_declaration(parser);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = peek(parser);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"compile", 7))
        {
            consume(parser, SBS_TOKEN_COMPILE);
            consume(parser, SBS_TOKEN_COLON);
            consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.flags = parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.extension = parse_string(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"defines", 7))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->compile.defines = parse_string_array(parser);
                }

                consume_if(parser, SBS_TOKEN_COMMA);
            }

            consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"archive", 7))
        {
            consume(parser, SBS_TOKEN_ARCHIVE);
            consume(parser, SBS_TOKEN_COLON);
            consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->archive.flags = parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->archive.extension = parse_string(parser);
                }

                consume_if(parser, SBS_TOKEN_COMMA);
            }

            consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"shared", 6))
        {
            consume(parser, SBS_TOKEN_SHARED);
            consume(parser, SBS_TOKEN_COLON);
            consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->shared.flags = parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->shared.extension = parse_string(parser);
                }

                consume_if(parser, SBS_TOKEN_COMMA);
            }

            consume(parser, SBS_TOKEN_RBRACE);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"executable", 10))
        {
            consume(parser, SBS_TOKEN_EXECUTABLE);
            consume(parser, SBS_TOKEN_COLON);
            consume(parser, SBS_TOKEN_LBRACE);
            
            while ((token = peek(parser)) && token->type != SBS_TOKEN_RBRACE)
            {
                if (fl_slice_equals_sequence(&token->value, (FlByte*)"flags", 5))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->executable.flags = parse_string_array(parser);
                }
                else if (fl_slice_equals_sequence(&token->value, (FlByte*)"extension", 9))
                {
                    consume(parser, SBS_TOKEN_IDENTIFIER);
                    consume(parser, SBS_TOKEN_COLON);
                    configuration->executable.extension = parse_string(parser);
                }

                consume_if(parser, SBS_TOKEN_COMMA);
            }

            consume(parser, SBS_TOKEN_RBRACE);
        }
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return configuration;
}

struct SbsEnv* parse_env_declaration(struct Parser *parser)
{
    struct SbsEnv *env = fl_malloc(sizeof(struct SbsEnv));

    // Consume 'env'
    consume(parser, SBS_TOKEN_ENV);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = consume(parser, SBS_TOKEN_IDENTIFIER);

    env->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    consume(parser, SBS_TOKEN_LBRACE);

    while (peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = peek(parser);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"args", 4))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            env->args = parse_string_array(parser);
            if (env->args)
            {
                size_t length = fl_array_length(env->args);
                env->args = fl_array_resize(env->args, length + 1);
                // argv[argc] must be NULL
                env->args[length] = NULL;
            }
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"type", 4))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            env->type = parse_identifier(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"terminal", 8))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            env->terminal = parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"variables", 9))
        {
            consume(parser, SBS_TOKEN_IDENTIFIER);
            consume(parser, SBS_TOKEN_COLON);
            env->variables = parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            env->actions = parse_actions(parser);
        }

        consume_if(parser, SBS_TOKEN_COMMA);
    }

    consume(parser, SBS_TOKEN_RBRACE);

    return env;
}

void merge_hashtable(FlHashtable dest, FlHashtable src)
{
    char **keys = fl_hashtable_keys(src);

    if (!keys)
        return;

    for (size_t i=0; i < fl_array_length(keys); i++)
    {
        fl_hashtable_add(dest, keys[i], fl_hashtable_get(src, keys[i]));
        fl_hashtable_remove(src, keys[i], false);
    }

    // It is ok to free the keys as the "dest" hashtable uses the fl_container_allocator_string
    // and will duplicate the keys
    fl_array_delete_each(keys, free_string);
}

void merge_into_file(struct SbsFile *dest_file, struct SbsFile *src_file)
{
    if (src_file == NULL)
        return;

    merge_hashtable(dest_file->actions, src_file->actions);
    merge_hashtable(dest_file->configurations, src_file->configurations);
    merge_hashtable(dest_file->envs, src_file->envs);
    merge_hashtable(dest_file->presets, src_file->presets);
    merge_hashtable(dest_file->targets, src_file->targets);
    merge_hashtable(dest_file->toolchains, src_file->toolchains);
}

bool parse_include_statement(struct Parser *parser, struct SbsFile *file)
{
    consume(parser, SBS_TOKEN_INCLUDE);
    char **includes = parse_string_array(parser);

    if (!includes)
        return false;

    // We need to get the current working directory based on the main SbsFile
    char *current_dir = NULL;

    FlVector parts = fl_cstring_split_by(file->filename, "/");
    fl_vector_pop(parts, NULL);

    if (fl_vector_length(parts) > 0)
    {
        current_dir = fl_cstring_join(parts, "/");
        fl_cstring_append(&current_dir, "/");
    }
    else
    {
        current_dir = fl_cstring_dup("./");
    }

    fl_vector_delete(parts);

    for (size_t i=0; i < fl_array_length(includes); i++)
    {
        char *filename = fl_cstring_vdup("%s%s", current_dir, includes[i]);
        struct SbsFile *included_file = sbs_file_parse(filename);
        merge_into_file(file, included_file);
        sbs_file_delete(included_file);
        fl_cstring_delete(filename);
    }

    fl_cstring_delete(current_dir);
    fl_array_delete_each(includes, free_string);

    return true;
}

bool parse_file(struct Parser *parser, struct SbsFile *file)
{
    const struct SbsToken *token = peek(parser);

    if (token->type == SBS_TOKEN_INCLUDE)
    {
        if (!parse_include_statement(parser, file))
            return false;
    }
    else if (token->type == SBS_TOKEN_ENV)
    {
        const struct SbsEnv *env = parse_env_declaration(parser);
        if (fl_hashtable_has_key(file->envs, env->name))
        {
            printf("Env %s cannot be redefined\n", env->name);
            free_env((struct SbsEnv*)env);
            return false;
        }
        fl_hashtable_add(file->envs, env->name, env);
    }
    else if (token->type == SBS_TOKEN_COMPILE || token->type == SBS_TOKEN_ARCHIVE || token->type == SBS_TOKEN_SHARED || token->type == SBS_TOKEN_EXECUTABLE)
    {
        const struct SbsTarget *target = parse_target_declaration(parser);
        if (fl_hashtable_has_key(file->targets, target->name))
        {
            printf("Target %s cannot be redefined\n", target->name);
            free_target((struct SbsTarget*)target);
            return false;
        }
        fl_hashtable_add(file->targets, target->name, target);
    }
    else if (token->type == SBS_TOKEN_TOOLCHAIN)
    {
        const struct SbsToolchain *toolchain = parse_toolchain_declaration(parser);
        if (fl_hashtable_has_key(file->toolchains, toolchain->name))
        {
            printf("Toolchain %s cannot be redefined\n", toolchain->name);
            free_toolchain((struct SbsToolchain*)toolchain);
            return false;
        }
        fl_hashtable_add(file->toolchains, toolchain->name, toolchain);
    }
    else if (token->type == SBS_TOKEN_CONFIG)
    {
        const struct SbsConfiguration *configuration = parse_configuration_declaration(parser);
        if (fl_hashtable_has_key(file->configurations, configuration->name))
        {
            printf("Configuration %s cannot be redefined\n", configuration->name);
            free_configuration((struct SbsConfiguration*)configuration);
            return false;
        }
        fl_hashtable_add(file->configurations, configuration->name, configuration);
    }
    else if (token->type == SBS_TOKEN_ACTION)
    {
        const struct SbsAction *action = parse_action_declaration(parser);
        if (fl_hashtable_has_key(file->actions, action->name))
        {
            printf("Action %s cannot be redefined\n", action->name);
            free_action((struct SbsAction*)action);
            return false;
        }
        fl_hashtable_add(file->actions, action->name, action);
    }
    else if (token->type == SBS_TOKEN_PRESET)
    {
        const struct SbsPreset *preset = parse_preset_declaration(parser);
        if (fl_hashtable_has_key(file->presets, preset->name))
        {
            printf("Preset %s cannot be redefined\n", preset->name);
            free_preset((struct SbsPreset*)preset);
            return false;
        }
        fl_hashtable_add(file->presets, preset->name, preset);
    }
    else
    {
        exit_with_unexpected_token(token);
    }

    return true;
}

void init_file(struct SbsFile *file)
{
    file->envs = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_env
    });

    file->targets = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_target
    });

    file->toolchains = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_toolchain
    });

    file->configurations = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_configuration
    });

    file->actions = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_action
    });

    file->presets = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_preset
    });
}

bool sbs_parser_parse(struct SbsLexer *lexer, struct SbsFile *file)
{
    init_file(file);

    struct Parser parser;

    // TODO: We can use a buffer here and use the sbs_lexer_next function
    parser.tokens = sbs_lexer_tokenize(lexer);
    parser.index = 0;
    parser.length = fl_array_length((FlArray)parser.tokens);

    bool success = true;
    while (has_input(&parser))
    {
        success = parse_file(&parser, file) && success;
        if (!success)
            break;
    }

    // Clean
    fl_array_delete((void*)parser.tokens);

    return success;
}
