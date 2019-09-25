#include "toolchain.h"
#include "common.h"
#include "parser.h"

#define BASE_TOOLCHAIN_KEY "#base"

void sbs_toolchain_entry_free(struct SbsToolchainEntry *toolchain_entry)
{
    if (toolchain_entry->compiler)
        fl_cstring_delete(toolchain_entry->compiler);

    if (toolchain_entry->archiver)
        fl_cstring_delete(toolchain_entry->archiver);

    if (toolchain_entry->linker)
        fl_cstring_delete(toolchain_entry->linker);
    
    fl_free(toolchain_entry);
}

void sbs_toolchain_free(struct SbsToolchainSection *toolchain)
{
    fl_cstring_delete(toolchain->name);

    if (toolchain->entries)
    {
        // We share the same struct SbsToolchainEntry between multiple environments, so we nullify 
        // duplicates, release the objects, and finally release the memory used by the array returned
        // by fl_hashtable_values
        struct SbsToolchainEntry **toolchains = fl_hashtable_values(toolchain->entries);

        size_t count = fl_array_length(toolchains);
        for (size_t i=0; i < count; i++)
        {
            for (size_t j=0; j < count; j++)
            {
                if (i == j)
                    continue;

                if (toolchains[i] == toolchains[j])
                    toolchains[j] = NULL;
            }
        }

        for (size_t i=0; i < count; i++)
        {
            if (toolchains[i] == NULL)
                continue;

            sbs_toolchain_entry_free(toolchains[i]);
        }

        fl_array_delete(toolchains);

        // Delete the hashtable including the keys (we already deleted the values)
        fl_hashtable_delete(toolchain->entries);
    }

    fl_free(toolchain);
}

static void parse_toolchain_entry(struct SbsParser *parser, struct SbsToolchainEntry *toolchain)
{
    while (sbs_parser_peek(parser)->type == SBS_TOKEN_IDENTIFIER)
    {
        const struct SbsToken *property = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        sbs_parser_consume(parser, SBS_TOKEN_COLON);

        const struct SbsToken *value = sbs_parser_peek(parser);

        if (fl_slice_equals_sequence(&property->value, (FlByte*)"compiler", 8))
        {
            toolchain->compiler = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&property->value, (FlByte*)"archiver", 8))
        {
            toolchain->archiver = sbs_common_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&property->value, (FlByte*)"linker", 6))
        {
            toolchain->linker = sbs_common_parse_string(parser);
        }
        else
        {
            sbs_parser_error(value, "while parsing a toolchain block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

/*
 * Function: sbs_toolchain_parse
 *  Parses a *toolchain* block which supports the following properties:
 *      - compiler: The executable path to the compiler
 *      - archiver: The executable path to the archiver
 *      - linker: The executable path to the linker
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  static struct SbsToolchainSection* - The parsed *toolchain* block
 *
 */
struct SbsToolchainSection* sbs_toolchain_parse(struct SbsParser *parser)
{
    struct SbsToolchainSection *toolchain = fl_malloc(sizeof(struct SbsToolchainSection));

    toolchain->entries = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsToolchainEntry *base_toolchain_entry = fl_malloc(sizeof(struct SbsToolchainEntry));

    // Use the special key #base for the default toolchain
    fl_hashtable_add(toolchain->entries, BASE_TOOLCHAIN_KEY, base_toolchain_entry);

    // Consume 'toolchain'
    sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    toolchain->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        {
            const struct SbsToken *token = sbs_parser_peek(parser);

            // Parse the for declaration
            char **envs = sbs_common_parse_for_declaration(parser);

            // Parse the toolchain info
            struct SbsToolchainEntry *toolchain_entry = fl_malloc(sizeof(struct SbsToolchainEntry));
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_toolchain_entry(parser, toolchain_entry);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
            
            for (size_t i=0; i < fl_array_length(envs); i++)
            {
                char *env = envs[i];

                if (flm_cstring_equals(BASE_TOOLCHAIN_KEY, env))
                    flm_vexit(ERR_FATAL, BASE_TOOLCHAIN_KEY " is a reserved keyword and cannot be used as an environment name (in line %ld, column %ld)\n", token->line, token->col);

                fl_hashtable_add(toolchain->entries, env, toolchain_entry);
            }

            fl_array_delete_each(envs, sbs_common_free_string);
        }
        else
        {
            parse_toolchain_entry(parser, base_toolchain_entry);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return toolchain;
}

const char* sbs_toolchain_get_compiler(const struct SbsToolchainSection *toolchain, const struct SbsEnv *env)
{
    const char *compiler = NULL;

    struct SbsToolchainEntry *base_tc = fl_hashtable_get(toolchain->entries, BASE_TOOLCHAIN_KEY);
    
    if (!base_tc)
        return compiler; // NULL

    compiler = base_tc->compiler;

    if (!fl_hashtable_has_key(toolchain->entries, env->name))
        return compiler;
    
    struct SbsToolchainEntry *env_tc = fl_hashtable_get(toolchain->entries, env->name);

    if (!env_tc || !env_tc->compiler)
        return compiler;

    compiler = env_tc->compiler;

    return compiler;
}

const char* sbs_toolchain_get_archiver(const struct SbsToolchainSection *toolchain, const struct SbsEnv *env)
{
    const char *archiver = NULL;

    struct SbsToolchainEntry *base_tc = fl_hashtable_get(toolchain->entries, BASE_TOOLCHAIN_KEY);
    
    if (!base_tc)
        return archiver; // NULL

    archiver = base_tc->archiver;

    if (!fl_hashtable_has_key(toolchain->entries, env->name))
        return archiver;
    
    struct SbsToolchainEntry *env_tc = fl_hashtable_get(toolchain->entries, env->name);

    if (!env_tc || !env_tc->archiver)
        return archiver;

    archiver = env_tc->archiver;

    return archiver;
}
