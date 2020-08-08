#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target-archive.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "cstring.h"
#include "parser.h"
#include "../utils.h"

SbsSectionArchive* sbs_section_archive_new(const struct FlSlice *name)
{
    SbsSectionArchive *section = fl_malloc(sizeof(SbsSectionArchive));

    section->base.type = SBS_SECTION_TARGET_ARCHIVE;
    section->base.name = sbs_slice_to_cstring(name);
    section->entries = fl_array_new(sizeof(SbsNodeArchive*), 0);

    return section;
}

SbsNodeArchive* sbs_section_archive_add_node(SbsSectionArchive *section)
{
    SbsNodeArchive *node = fl_malloc(sizeof(SbsNodeArchive));

    section->entries = fl_array_append(section->entries, &node);

    return node;
}

void sbs_node_archive_free(SbsNodeArchive *node)
{
    sbs_node_target_free_members((SbsAbstractNodeTarget*) node);

    if (node->objects)
        fl_array_free_each_pointer(node->objects, (FlArrayFreeElementFunc) sbs_source_free);

    if (node->output_name)
        fl_cstring_free(node->output_name);    

    fl_free(node);
}

void sbs_section_archive_free(SbsSectionArchive *section)
{
    sbs_section_target_free_members((SbsAbstractSectionTarget*) section);

    if (section->entries)
        fl_array_free_each_pointer(section->entries, (FlArrayFreeElementFunc) sbs_node_archive_free);

    fl_free(section);
}

void sbs_section_archive_body_parse(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_source_array_parse(parser);
        }
        else if (sbs_token_equals(token, "output_name"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_cstring_parse(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_cstring_parse(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (token->type == SBS_TOKEN_IF)
        {
            sbs_section_target_if_stmt_parse(parser, (SbsAbstractSectionTarget*) target_section, SBS_SECTION_TARGET_ARCHIVE);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing a target archive block");
        }

        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }
}

SbsAbstractSectionTarget* sbs_section_archive_parse(SbsParser *parser)
{
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    SbsSectionArchive *target_section = sbs_section_archive_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    sbs_section_archive_body_parse(parser, target_section, sbs_section_archive_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}
