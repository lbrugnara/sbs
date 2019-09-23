#include "toolchain.h"
#include "common.h"
#include "parser.h"

void sbs_toolchain_info_free(struct SbsToolchainInfo *toolchain_info)
{
    if (toolchain_info->compiler)
        fl_cstring_delete(toolchain_info->compiler);

    if (toolchain_info->archiver)
        fl_cstring_delete(toolchain_info->archiver);

    if (toolchain_info->linker)
        fl_cstring_delete(toolchain_info->linker);
    
    fl_free(toolchain_info);
}

void sbs_toolchain_free(struct SbsToolchain *toolchain)
{
    fl_cstring_delete(toolchain->name);

    if (toolchain->toolchains)
    {
        // We share the same struct SbsToolchainInfo between multiple environments, so we nullify 
        // duplicates, release the objects, and finally release the memory used by the array returned
        // by fl_hashtable_values
        struct SbsToolchainInfo **toolchains = fl_hashtable_values(toolchain->toolchains);

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

            sbs_toolchain_info_free(toolchains[i]);
        }

        fl_array_delete(toolchains);

        // Delete the hashtable including the keys (we already deleted the values)
        fl_hashtable_delete(toolchain->toolchains);
    }

    fl_free(toolchain);
}

static void free_map_entry(void *value)
{
    sbs_toolchain_free((struct SbsToolchain*)value);
}

void sbs_toolchain_map_init(FlHashtable *toolchains)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_map_entry
    };
    
    *toolchains = fl_hashtable_new_args(new_args);
}

static void parse_toolchain_info(struct SbsParser *parser, struct SbsToolchainInfo *toolchain)
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
 *  static struct SbsToolchain* - The parsed *toolchain* block
 *
 */
struct SbsToolchain* sbs_toolchain_parse(struct SbsParser *parser)
{
    struct SbsToolchain *toolchain = fl_malloc(sizeof(struct SbsToolchain));

    toolchain->toolchains = fl_hashtable_new_args((struct FlHashtableArgs){
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer
    });

    struct SbsToolchainInfo *base_toolchain_info = fl_malloc(sizeof(struct SbsToolchainInfo));

    // Use the special key __base__ for the default toolchain
    fl_hashtable_add(toolchain->toolchains, "__base__", base_toolchain_info);

    // Consume 'toolchain'
    sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    toolchain->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        if (sbs_parser_peek(parser)->type == SBS_TOKEN_IDENTIFIER)
        {
            parse_toolchain_info(parser, base_toolchain_info);
        }
        else if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        {
            const struct SbsToken *token = sbs_parser_peek(parser);

            // Parse the for declaration
            char **envs = sbs_common_parse_for_declaration(parser);

            // Parse the toolchain info
            struct SbsToolchainInfo *toolchain_info = fl_malloc(sizeof(struct SbsToolchainInfo));
            sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
            parse_toolchain_info(parser, toolchain_info);
            sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
            
            for (size_t i=0; i < fl_array_length(envs); i++)
            {
                char *env = envs[i];

                if (flm_cstring_equals("__base__", env))
                    flm_vexit(ERR_FATAL, "__base__ is a reserved keyword and cannot be used as an environment name (in line %ld, column %ld)\n", token->line, token->col);

                fl_hashtable_add(toolchain->toolchains, env, toolchain_info);
            }

            fl_array_delete_each(envs, sbs_common_free_string);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return toolchain;
}

const char* sbs_toolchain_get_compiler(const struct SbsToolchain *toolchain, const struct SbsEnv *env)
{
    const char *compiler = NULL;

    struct SbsToolchainInfo *base_tc = fl_hashtable_get(toolchain->toolchains, "__base__");
    
    if (!base_tc)
        return compiler; // NULL

    compiler = base_tc->compiler;

    if (!fl_hashtable_has_key(toolchain->toolchains, env->name))
        return compiler;
    
    struct SbsToolchainInfo *env_tc = fl_hashtable_get(toolchain->toolchains, env->name);

    if (!env_tc || !env_tc->compiler)
        return compiler;

    compiler = env_tc->compiler;

    return compiler;
}

const char* sbs_toolchain_get_archiver(const struct SbsToolchain *toolchain, const struct SbsEnv *env)
{
    const char *archiver = NULL;

    struct SbsToolchainInfo *base_tc = fl_hashtable_get(toolchain->toolchains, "__base__");
    
    if (!base_tc)
        return archiver; // NULL

    archiver = base_tc->archiver;

    if (!fl_hashtable_has_key(toolchain->toolchains, env->name))
        return archiver;
    
    struct SbsToolchainInfo *env_tc = fl_hashtable_get(toolchain->toolchains, env->name);

    if (!env_tc || !env_tc->archiver)
        return archiver;

    archiver = env_tc->archiver;

    return archiver;
}
