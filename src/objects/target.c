#include <fllib.h>
#include "target.h"
#include "common.h"
#include "parser.h"

#define BASE_TARGET_KEY "#base"

static void parse_for_section(struct SbsParser *parser, struct SbsTargetSection *target_section, enum SbsTargetType target_type);

void sbs_target_entry_free(enum SbsTargetType target_type, struct SbsTargetNode *target_entry)
{
    sbs_actions_node_free(&target_entry->actions);

    if (target_entry->output_dir)
        fl_cstring_delete(target_entry->output_dir);

    switch (target_type)
    {
        case SBS_TARGET_COMPILE:
        {
            struct SbsTargetCompileNode *compile = (struct SbsTargetCompileNode*)target_entry;
            
            if (compile->sources)
                fl_array_delete_each(compile->sources, sbs_common_free_string);

            if (compile->includes)
                fl_array_delete_each(compile->includes, sbs_common_free_string);

            if (compile->defines)
                fl_array_delete_each(compile->defines, sbs_common_free_string);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            struct SbsTargetArchiveNode *archive = (struct SbsTargetArchiveNode*)target_entry;

            if (archive->objects)
                fl_array_delete_each(archive->objects, sbs_common_free_string_or_id);

            if (archive->output_name)
                fl_cstring_delete(archive->output_name);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            struct SbsTargetSharedNode *shared = (struct SbsTargetSharedNode*)target_entry;

            if (shared->objects)
                fl_array_delete_each(shared->objects, sbs_common_free_string_or_id);

            if (shared->output_name)
                fl_cstring_delete(shared->output_name);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            struct SbsTargetExecutableNode *executable = (struct SbsTargetExecutableNode*)target_entry;

            if (executable->objects)
                fl_array_delete_each(executable->objects, sbs_common_free_string_or_id);

            if (executable->output_name)
                fl_cstring_delete(executable->output_name);

            break;
        }
    }

    fl_free(target_entry);
}

void sbs_target_free(struct SbsTargetSection *target_section)
{
    fl_cstring_delete(target_section->name);

    if (target_section->entries)
    {
        // We share the same struct SbsTargetNode between multiple environments, so we nullify 
        // duplicates, release the objects, and finally release the memory used by the array returned
        // by fl_hashtable_values
        struct SbsTargetNode **target_entries = fl_hashtable_values(target_section->entries);

        size_t count = fl_array_length(target_entries);
        for (size_t i=0; i < count; i++)
        {
            for (size_t j=0; j < count; j++)
            {
                if (i == j)
                    continue;

                if (target_entries[i] == target_entries[j])
                    target_entries[j] = NULL;
            }
        }

        for (size_t i=0; i < count; i++)
        {
            if (target_entries[i] == NULL)
                continue;

            sbs_target_entry_free(target_section->type, target_entries[i]);
        }

        fl_array_delete(target_entries);

        // Delete the hashtable including the keys (we already deleted the values)
        fl_hashtable_delete(target_section->entries);
    }

    fl_free(target_section);
}

static void parse_compile_body(struct SbsParser *parser, struct SbsTargetSection *target_section, struct SbsTargetCompileNode *target)
{
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
            target->base.actions = sbs_actions_node_parse(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"defines", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_common_parse_string_array(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, target_section, SBS_TARGET_COMPILE);
        }
        else
        {
            sbs_parser_error(token, "while parsing a target compile block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_target_parse_compile
 *  Parses a *compile* target block which supports the following properties:
 *      - includes: List of files to include in the compile process
 *      - sources: List of source files to compile
 *      - output_dir: Target's output dir
 *      - actions: Actions to run before and after the link process
 *      - defines: List of C defines to include in the compile process
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsTargetSection* - Parsed *compile* target
 *
 */
struct SbsTargetSection* sbs_target_parse_compile(struct SbsParser *parser)
{
    struct SbsTargetSection *target_section = fl_malloc(sizeof(struct SbsTargetSection));
    target_section->type = SBS_TARGET_COMPILE;
    target_section->entries = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsTargetCompileNode *target_entry = fl_malloc(sizeof(struct SbsTargetCompileNode));
    fl_hashtable_add(target_section->entries, BASE_TARGET_KEY, target_entry);

    // Consume the 'compile' token
    sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);    
    parse_compile_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTargetSection*)target_section;
}

static void parse_archive_body(struct SbsParser *parser, struct SbsTargetSection *target_section, struct SbsTargetArchiveNode *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
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
            target->base.actions = sbs_actions_node_parse(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, target_section, SBS_TARGET_ARCHIVE);
        }
        else
        {
            sbs_parser_error(token, "while parsing a target archive block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
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
 *  struct SbsTargetSection* - Parsed *archive* target
 *
 */
struct SbsTargetSection* sbs_target_parse_archive(struct SbsParser *parser)
{
    struct SbsTargetSection *target_section = fl_malloc(sizeof(struct SbsTargetSection));
    target_section->type = SBS_TARGET_ARCHIVE;
    target_section->entries = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsTargetArchiveNode *target_entry = fl_malloc(sizeof(struct SbsTargetArchiveNode));
    fl_hashtable_add(target_section->entries, BASE_TARGET_KEY, target_entry);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_archive_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTargetSection*)target_section;
}

static void parse_shared_body(struct SbsParser *parser, struct SbsTargetSection *target_section, struct SbsTargetSharedNode *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
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
            target->base.actions = sbs_actions_node_parse(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, target_section, SBS_TARGET_SHARED);
        }
        else
        {
            sbs_parser_error(token, "while parsing a target shared block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
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
 *  struct SbsTargetSection* - Parsed *shared* target
 *
 */
struct SbsTargetSection* sbs_target_parse_shared(struct SbsParser *parser)
{
    struct SbsTargetSection *target_section = fl_malloc(sizeof(struct SbsTargetSection));
    target_section->type = SBS_TARGET_SHARED;
    target_section->entries = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsTargetSharedNode *target_entry = fl_malloc(sizeof(struct SbsTargetSharedNode));
    fl_hashtable_add(target_section->entries, BASE_TARGET_KEY, target_entry);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_shared_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTargetSection*)target_section;
}

static void parse_executable_body(struct SbsParser *parser, struct SbsTargetSection *target_section, struct SbsTargetExecutableNode *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
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
            target->base.actions = sbs_actions_node_parse(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, target_section, SBS_TARGET_EXECUTABLE);
        }
        else
        {
            sbs_parser_error(token, "while parsing a target executable block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
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
 *  struct SbsTargetSection* - Parsed *executable* target
 *
 */
struct SbsTargetSection* sbs_target_parse_executable(struct SbsParser *parser)
{
    struct SbsTargetSection *target_section = fl_malloc(sizeof(struct SbsTargetSection));
    target_section->type = SBS_TARGET_EXECUTABLE;
    target_section->entries = fl_hashtable_new_args((struct FlHashtableArgs) {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsTargetExecutableNode *target_entry = fl_malloc(sizeof(struct SbsTargetExecutableNode));
    fl_hashtable_add(target_section->entries, BASE_TARGET_KEY, target_entry);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_executable_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (struct SbsTargetSection*)target_section;
}

static void parse_for_section(struct SbsParser *parser, struct SbsTargetSection *target_section, enum SbsTargetType target_type)
{
    const struct SbsToken *token = sbs_parser_peek(parser);

    // Parse the for declaration
    char **envs = sbs_common_parse_for_declaration(parser);
    
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    // Parse the configuration info
    struct SbsTargetNode *target_entry = NULL;

    if (target_type == SBS_TARGET_COMPILE)
    {
        target_entry = fl_malloc(sizeof(struct SbsTargetCompileNode));
        parse_compile_body(parser, target_section, (struct SbsTargetCompileNode*)target_entry);
    }
    else if (target_type == SBS_TARGET_ARCHIVE)
    {
        target_entry = fl_malloc(sizeof(struct SbsTargetArchiveNode));
        parse_archive_body(parser, target_section, (struct SbsTargetArchiveNode*)target_entry);
    }
    else if (target_type == SBS_TARGET_SHARED)
    {
        target_entry = fl_malloc(sizeof(struct SbsTargetSharedNode));
        parse_shared_body(parser, target_section, (struct SbsTargetSharedNode*)target_entry);
    }
    else if (target_type == SBS_TARGET_EXECUTABLE)
    {
        target_entry = fl_malloc(sizeof(struct SbsTargetExecutableNode));
        parse_executable_body(parser, target_section, (struct SbsTargetExecutableNode*)target_entry);
    }
    else
    {
        sbs_parser_error(token, "while parsing a for statement in a target section");
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
    
    for (size_t i=0; i < fl_array_length(envs); i++)
    {
        char *env = envs[i];

        if (flm_cstring_equals(BASE_TARGET_KEY, env))
            flm_vexit(ERR_FATAL, BASE_TARGET_KEY " is a reserved keyword and cannot be used as an environment name (in line %ld, column %ld)\n", token->line, token->col);

        fl_hashtable_add(target_section->entries, env, target_entry);
    }

    fl_array_delete_each(envs, sbs_common_free_string);
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
 *  struct SbsTargetSection* - The parsed *target*
 *
 */
struct SbsTargetSection* sbs_target_parse(struct SbsParser *parser)
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
            sbs_parser_error(token, "while parsing a target block");
    }

    return NULL;
}

static void merge_base_target(struct SbsTarget *extend, const struct SbsTargetNode *source)
{
    if (source->output_dir)
        extend->output_dir = source->output_dir;

    if (source->actions.before)
        extend->actions.before = sbs_common_extend_array(extend->actions.before, source->actions.before);

    if (source->actions.after)
        extend->actions.after = sbs_common_extend_array(extend->actions.after, source->actions.after);
}

static void merge_compile_target(struct SbsTargetCompile *extend, const struct SbsTargetCompileNode *source)
{
    if (source->includes)
        extend->includes = sbs_common_extend_array(extend->includes, source->includes);

    if (source->sources)
        extend->sources = sbs_common_extend_array(extend->sources, source->sources);

    if (source->defines)
        extend->defines = sbs_common_extend_array(extend->defines, source->defines);
}

static void merge_archive_target(struct SbsTargetArchive *extend, const struct SbsTargetArchiveNode *source)
{
    if (source->output_name)
        extend->output_name = source->output_name;

    if (source->objects)
        extend->objects = sbs_common_extend_array(extend->objects, source->objects);
}

static void merge_shared_target(struct SbsTargetShared *extend, const struct SbsTargetSharedNode *source)
{
    if (source->output_name)
        extend->output_name = source->output_name;

    if (source->objects)
        extend->objects = sbs_common_extend_array(extend->objects, source->objects);
}

static void merge_executable_target(struct SbsTargetExecutable *extend, const struct SbsTargetExecutableNode *source)
{
    if (source->output_name)
        extend->output_name = source->output_name;

    if (source->objects)
        extend->objects = sbs_common_extend_array(extend->objects, source->objects);
}

struct SbsTarget* sbs_target_resolve(const char *target_name, FlHashtable target_map, const char *env_name)
{    
    const struct SbsTargetSection *target_section = fl_hashtable_get(target_map, target_name);

    if (!target_section)
        return NULL;

    struct SbsTarget *target_obj = NULL;
    switch (target_section->type)
    {
        case SBS_TARGET_COMPILE:
            target_obj = fl_malloc(sizeof(struct SbsTargetCompile));
            break;
        case SBS_TARGET_ARCHIVE:
            target_obj = fl_malloc(sizeof(struct SbsTargetArchive));
            break;
        case SBS_TARGET_SHARED:
            target_obj = fl_malloc(sizeof(struct SbsTargetShared));
            break;
        case SBS_TARGET_EXECUTABLE:
            target_obj = fl_malloc(sizeof(struct SbsTargetExecutable));
            break;
        default:
            return NULL;
    }

    target_obj->name = target_section->name;
    target_obj->type = target_section->type;

    FlList hierarchy = fl_list_new();

    // Using prepend we will keep the list ordered
    if (fl_hashtable_has_key(target_section->entries, env_name))
        fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, env_name));

    fl_list_prepend(hierarchy, fl_hashtable_get(target_section->entries, BASE_TARGET_KEY));

    struct FlListNode *node = fl_list_head(hierarchy);
    while (node)
    {
        const struct SbsTargetNode *ancestor = (const struct SbsTargetNode*)node->value;
        
        merge_base_target(target_obj, ancestor);

        switch (target_section->type)
        {
            case SBS_TARGET_COMPILE:
                merge_compile_target((struct SbsTargetCompile*)target_obj, (struct SbsTargetCompileNode*)node->value);
                break;
            case SBS_TARGET_ARCHIVE:
                merge_archive_target((struct SbsTargetArchive*)target_obj, (struct SbsTargetArchiveNode*)node->value);
                break;
            case SBS_TARGET_SHARED:
                merge_shared_target((struct SbsTargetShared*)target_obj, (struct SbsTargetSharedNode*)node->value);
                break;
            case SBS_TARGET_EXECUTABLE:
                merge_executable_target((struct SbsTargetExecutable*)target_obj, (struct SbsTargetExecutableNode*)node->value);
                break;
            default:
                return NULL;
        }

        node = node->next;
    }

    fl_list_delete(hierarchy);

    return target_obj;
}

void sbs_target_release(struct SbsTarget *target)
{
    sbs_actions_node_free(&target->actions);

    switch (target->type)
    {
        case SBS_TARGET_COMPILE:
        {
            struct SbsTargetCompile *compile = (struct SbsTargetCompile*)target;
            
            if (compile->sources)
                fl_array_delete(compile->sources);

            if (compile->includes)
                fl_array_delete(compile->includes);

            if (compile->defines)
                fl_array_delete(compile->defines);

            break;
        }
        case SBS_TARGET_ARCHIVE:
        {
            struct SbsTargetArchive *archive = (struct SbsTargetArchive*)target;

            if (archive->objects)
                fl_array_delete(archive->objects);

            break;
        }
        case SBS_TARGET_SHARED:
        {
            struct SbsTargetShared *shared = (struct SbsTargetShared*)target;

            if (shared->objects)
                fl_array_delete(shared->objects);

            break;
        }
        case SBS_TARGET_EXECUTABLE:
        {
            struct SbsTargetExecutable *executable = (struct SbsTargetExecutable*)target;

            if (executable->objects)
                fl_array_delete(executable->objects);

            break;
        }
    }

    fl_free(target);
}
