#include "toolchain.h"
#include "common.h"
#include "parser.h"

void sbs_toolchain_free(struct SbsToolchain *toolchain)
{
    fl_cstring_delete(toolchain->name);
    
    if (toolchain->extends)
        fl_array_delete_each(toolchain->extends, sbs_common_free_string);

    if (toolchain->for_envs)
        fl_array_delete_each(toolchain->for_envs, sbs_common_free_string);

    if (toolchain->compiler)
        fl_cstring_delete(toolchain->compiler);

    if (toolchain->archiver)
        fl_cstring_delete(toolchain->archiver);

    if (toolchain->linker)
        fl_cstring_delete(toolchain->linker);
    
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

    // Consume 'toolchain'
    sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
    
    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    toolchain->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_EXTENDS)
        toolchain->extends = sbs_common_parse_extends_declaration(parser);

    if (sbs_parser_peek(parser)->type == SBS_TOKEN_FOR)
        toolchain->for_envs = sbs_common_parse_for_declaration(parser);

    // toolchain_body -> 'compiler' ':' STRING ','?
    //                 | 'archiver' ':' STRING ','?
    //                 | 'linker' ':' STRING ','?
    //                 ;

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);

        const struct SbsToken *property = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
        sbs_parser_consume(parser, SBS_TOKEN_COLON);

        // So far, all properties are string 
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
            sbs_parser_error(value);
        }
        
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return toolchain;
}


void find_ancestors(const struct SbsToolchain *toolchain, FlList ancestors, FlHashtable toolchain_map)
{
    if (!toolchain->extends)
        return;

    size_t length = fl_array_length(toolchain->extends);
    for (size_t i=0; i < length; i++)
        fl_list_prepend(ancestors, fl_hashtable_get(toolchain_map, toolchain->extends[i]));
    
    for (size_t i=0; i < length; i++)
        find_ancestors(fl_hashtable_get(toolchain_map, toolchain->extends[i]), ancestors, toolchain_map);
}

void sbs_toolchain_inheritance_clean(struct SbsToolchain *extended_toolchain)
{
    // char **for_envs;
    if (extended_toolchain->for_envs)
        fl_array_delete(extended_toolchain->for_envs);
}

bool sbs_toolchain_inheritance_resolve(struct SbsToolchain *extended_toolchain, const char *toolchain_name, FlHashtable toolchain_map)
{
    const struct SbsToolchain *toolchain = fl_hashtable_get(toolchain_map, toolchain_name);

    if (!toolchain)
        return false;

    // Name will be always the targeted toolchain
    extended_toolchain->name = toolchain->name;

    FlList ancestors = fl_list_new();

    // Using prepend we will keep the list ordered
    fl_list_prepend(ancestors, toolchain);
    find_ancestors(toolchain, ancestors, toolchain_map);

    struct FlListNode *node = fl_list_head(ancestors);
    while (node)
    {
        const struct SbsToolchain *ancestor = (const struct SbsToolchain*)node->value;

        // char **for_toolchains;
        if (ancestor->for_envs)
            sbs_common_extend_fl_array(&(extended_toolchain->for_envs), ancestor->for_envs);

        //const char *compiler;
        if (ancestor->compiler)
            extended_toolchain->compiler = ancestor->compiler;

        //const char *linker;
        if (ancestor->linker)
            extended_toolchain->linker = ancestor->linker;

        //const char *archiver;
        if (ancestor->archiver)
            extended_toolchain->archiver = ancestor->archiver;        

        node = node->next;
    }

    fl_list_delete(ancestors);

    return true;
}
