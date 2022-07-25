#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include "target.h"
#include "target-compile.h"
#include "target-archive.h"
#include "target-shared.h"
#include "target-executable.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "expr.h"
#include "expr-if.h"
#include "../utils.h"

void sbs_node_target_free_members(SbsAbstractNodeTarget *node)
{
    sbs_property_actions_free(&node->actions);

    if (node->condition)
        sbs_expr_free(node->condition);

    if (node->output_dir)
        fl_cstring_free(node->output_dir);

    if (node->flags)
        fl_array_free_each_pointer(node->flags, (FlArrayFreeElementFunc) sbs_expr_free_string);
}

void sbs_section_target_free_members(SbsAbstractSectionTarget *section)
{
    if (section->name)
        fl_cstring_free(section->name);
}

void sbs_section_target_free(SbsAbstractSectionTarget *section)
{
    switch (section->type)
    {
        case SBS_SECTION_TARGET_COMPILE:
            sbs_section_compile_free((SbsSectionCompile*) section);
            break;
        case SBS_SECTION_TARGET_ARCHIVE:
            sbs_section_archive_free((SbsSectionArchive*) section);
            break;
        case SBS_SECTION_TARGET_SHARED:
            sbs_section_shared_free((SbsSectionShared*) section);
            break;
        case SBS_SECTION_TARGET_EXECUTABLE:
            sbs_section_executable_free((SbsSectionExecutable*) section);
            break;
    }
}

void sbs_section_target_if_stmt_parse(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type)
{
    const SbsToken *token = sbs_parser_peek(parser);

    // Parse the for declaration
    SbsExpression *condition = sbs_expr_parse_if(parser);
    
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    if (target_type == SBS_SECTION_TARGET_COMPILE)
    {
        SbsSectionCompile *compile_section = (SbsSectionCompile*) target_section;
        SbsNodeCompile *compile_entry = sbs_section_compile_add_node(compile_section);
        compile_entry->base.condition = condition;
        sbs_section_compile_body_parse(parser, compile_section, compile_entry);
    }
    else if (target_type == SBS_SECTION_TARGET_ARCHIVE)
    {
        SbsSectionArchive *archive_section = (SbsSectionArchive*) target_section;
        SbsNodeArchive *archive_entry = sbs_section_archive_add_node(archive_section);
        archive_entry->base.condition = condition;
        sbs_section_archive_body_parse(parser, archive_section, archive_entry);
    }
    else if (target_type == SBS_SECTION_TARGET_SHARED)
    {
        SbsSectionShared *shared_section = (SbsSectionShared*) target_section;
        SbsNodeShared *shared_entry = sbs_section_shared_add_node(shared_section);
        shared_entry->base.condition = condition;
        sbs_section_shared_body_parse(parser, shared_section, shared_entry);
    }
    else if (target_type == SBS_SECTION_TARGET_EXECUTABLE)
    {
        SbsSectionExecutable *executable_section = (SbsSectionExecutable*) target_section;
        SbsNodeExecutable *executable_entry = sbs_section_executable_add_node(executable_section);
        executable_entry->base.condition = condition;
        sbs_section_executable_body_parse(parser, executable_section, executable_entry);
    }
    else
    {
        sbs_parser_error(parser, token, "while parsing a for statement in a target section");
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
}

SbsAbstractSectionTarget* sbs_section_target_parse(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

    if (token == NULL)
        return NULL;

    switch (token->type)
    {
        case SBS_TOKEN_COMPILE:
            return sbs_section_compile_parse(parser);
        case SBS_TOKEN_ARCHIVE:
            return sbs_section_archive_parse(parser);
        case SBS_TOKEN_SHARED:
            return sbs_section_shared_parse(parser);
        case SBS_TOKEN_EXECUTABLE:
            return sbs_section_executable_parse(parser);
        default:
            sbs_parser_error(parser, token, "while parsing a target block");
    }

    return NULL;
}
