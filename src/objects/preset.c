#include "preset.h"
#include "common.h"
#include "parser.h"

void sbs_preset_free(struct SbsPreset *preset)
{
    fl_cstring_delete(preset->name);
    fl_cstring_delete(preset->env);
    fl_cstring_delete(preset->toolchain);
    fl_cstring_delete(preset->config);
    fl_cstring_delete(preset->target);

    if (preset->actions.before)
        fl_array_delete_each(preset->actions.before, sbs_common_free_string_or_id);

    if (preset->actions.after)
        fl_array_delete_each(preset->actions.after, sbs_common_free_string_or_id);

    fl_free(preset);
}

static void free_map_entry(void *value)
{
    sbs_preset_free((struct SbsPreset*)value);
}

void sbs_preset_map_init(FlHashtable *presets)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = free_map_entry
    };
    
    *presets = fl_hashtable_new_args(new_args);
}


/*
 * Function: sbs_preset_parse
 *  Parses a *preset* block which supports the following properties:
 *      - env: Environment name
 *      - toolchain: Toolchain name
 *      - config: Configuration name
 *      - target: Target name
 *      - actions: Actions to run before and after the build process
 * 
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  struct SbsPreset* - Parsed *preset* block
 *
 */
struct SbsPreset* sbs_preset_parse(struct SbsParser *parser)
{
    struct SbsPreset *preset = fl_malloc(sizeof(struct SbsPreset));

    // Consume the 'preset' token
    sbs_parser_consume(parser, SBS_TOKEN_PRESET);

    // Consume IDENTIFIER
    const struct SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    preset->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    const struct SbsToken *token = NULL;

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    while ((token = sbs_parser_peek(parser)) && token->type != SBS_TOKEN_RBRACE)
    {
        const struct SbsToken *token = sbs_parser_peek(parser);

        if (token->type == SBS_TOKEN_ENV)
        {
            sbs_parser_consume(parser, SBS_TOKEN_ENV);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->env = sbs_common_parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_TOOLCHAIN)
        {
            sbs_parser_consume(parser, SBS_TOKEN_TOOLCHAIN);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->toolchain = sbs_common_parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_CONFIG)
        {
            sbs_parser_consume(parser, SBS_TOKEN_CONFIG);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->config = sbs_common_parse_identifier(parser);
        }
        else if (token->type == SBS_TOKEN_TARGET)
        {
            sbs_parser_consume(parser, SBS_TOKEN_TARGET);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            preset->target = sbs_common_parse_identifier(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            preset->actions = sbs_actions_parse(parser);
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume_if(parser, SBS_TOKEN_RBRACE);

    return preset;
}
