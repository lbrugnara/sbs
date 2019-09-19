#include <fllib.h>
#include "target.h"
#include "common.h"
#include "parser.h"

void sbs_target_free(struct SbsTarget *target)
{
    fl_cstring_delete(target->name);

    if (target->actions.before)
        fl_array_delete_each(target->actions.before, sbs_common_free_string_or_id);

    if (target->actions.after)
        fl_array_delete_each(target->actions.after, sbs_common_free_string_or_id);

    if (target->output_dir)
        fl_cstring_delete(target->output_dir);


    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            struct SbsTargetCompile *compile = (struct SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_delete_each(compile->sources, sbs_common_free_string);

            if (compile->includes)
                fl_array_delete_each(compile->includes, sbs_common_free_string);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            struct SbsTargetArchive *archive = (struct SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_delete_each(archive->objects, sbs_common_free_string_or_id);

            if (archive->output_name)
                fl_cstring_delete(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            struct SbsTargetShared *shared = (struct SbsTargetShared*)target;

            if (shared->objects)
                fl_array_delete_each(shared->objects, sbs_common_free_string_or_id);

            if (shared->output_name)
                fl_cstring_delete(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            struct SbsTargetExecutable *executable = (struct SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_delete_each(executable->objects, sbs_common_free_string_or_id);

            if (executable->output_name)
                fl_cstring_delete(executable->output_name);

            break;
        }
    }

    fl_free(target);
}

static void free_map_entry(void *value)
{
    sbs_target_free((struct SbsTarget*)value);
}

void sbs_target_map_init(FlHashtable *targets)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_map_entry
    };
    
    *targets = fl_hashtable_new_args(new_args);
}

/*
 * Function: sbs_target_parse_compile
 *  Parses a *compile* target block which supports the following properties:
 *      - includes: List of files to include in the compile process
 *      - sources: List of source files to compile
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the link process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTarget* - Parsed *compile* target
 *
 */
struct SbsTarget* sbs_target_parse_compile(struct SbsParser *parser)
{
    struct SbsTargetCompile *target = fl_malloc(sizeof(struct SbsTargetCompile));
    target->base.type = SBS_TARGET_COMPILE;

    // Consume the 'compile' token
    sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = sbs_parser_peek(parser);
        
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"includes", 8))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->includes = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"sources", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->sources = sbs_common_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_actions_parse(parser);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

/*
 * Function: sbs_target_parse_archive
 *  Parses an *archive* target block which supports the following properties:
 *      - objects: List of object files to pass to the archiver
 *      - output_name: Target's output name
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the archive process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTarget* - Parsed *archive* target
 *
 */
struct SbsTarget* sbs_target_parse_archive(struct SbsParser *parser)
{
    struct SbsTargetArchive *target = fl_malloc(sizeof(struct SbsTargetArchive));
    target->base.type = SBS_TARGET_ARCHIVE;

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = sbs_parser_peek(parser);
        //const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_common_parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_actions_parse(parser);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

/*
 * Function: sbs_target_parse_shared
 *  Parses a *shared* target block which supports the following properties:
 *      - objects: List of object files to pass to the linker
 *      - output_name: Target's output name
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the link process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTarget* - Parsed *shared* target
 *
 */
struct SbsTarget* sbs_target_parse_shared(struct SbsParser *parser)
{
    struct SbsTargetShared *target = fl_malloc(sizeof(struct SbsTargetShared));
    target->base.type = SBS_TARGET_SHARED;

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = sbs_parser_peek(parser);
        //const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_common_parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_actions_parse(parser);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}

/*
 * Function: sbs_target_parse_executable
 *  Parses an *executable* target block which supports the following properties:
 *      - objects: List of object files to pass to the linker
 *      - output_name: Target's output name
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the link process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTarget* - Parsed *executable* target
 *
 */
struct SbsTarget* sbs_target_parse_executable(struct SbsParser *parser)
{
    struct SbsTargetExecutable *target = fl_malloc(sizeof(struct SbsTargetExecutable));
    target->base.type = SBS_TARGET_EXECUTABLE;

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target->base.name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *token = sbs_parser_peek(parser);
        //const struct SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_common_parse_string_or_id_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_actions_parse(parser);
        }
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTarget*)target;
}


/*
 * Function: sbs_target_parse
 *  The root function to parse a *target* declaration. It just calls the
 *  specific function based on the token
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTarget* - The parsed *target*
 *
 */
struct SbsTarget* sbs_target_parse(struct SbsParser *parser)
{
    const struct SbsToken *token = sbs_parser_peek(parser);

    if (token == NULL)
        return NULL;

    switch (token->type)
    {
        case SBS_TOKEN_COMPILE:
            return sbs_target_parse_compile(parser);
        case SBS_TOKEN_ARCHIVE:
            return sbs_target_parse_archive(parser);
        case SBS_TOKEN_SHARED:
            return sbs_target_parse_shared(parser);
        case SBS_TOKEN_EXECUTABLE:
            return sbs_target_parse_executable(parser);
        default:
            sbs_parser_error(token);
    }

    return NULL;
}
