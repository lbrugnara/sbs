#include "target.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "conditional.h"
#include "target-compile.h"
#include "../target-compile.h"
#include "target-archive.h"
#include "../target-archive.h"
#include "target-shared.h"
#include "../target-shared.h"
#include "target-executable.h"
#include "../target-executable.h"
#include "../../utils.h"

void sbs_section_target_for_condition_parse(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type)
{
    const SbsToken *token = sbs_parser_peek(parser);

    // Parse the for declaration
    SbsStmtConditional *condition = sbs_stmt_conditional_parse(parser);
    
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    if (target_type == SBS_TARGET_COMPILE)
    {
        SbsSectionCompile *compile_section = (SbsSectionCompile*) target_section;
        SbsNodeCompile *compile_entry = sbs_section_compile_add_node(compile_section);
        compile_entry->base.condition = condition;
        sbs_section_compile_body_parse(parser, compile_section, compile_entry);
    }
    else if (target_type == SBS_TARGET_ARCHIVE)
    {
        SbsSectionArchive *archive_section = (SbsSectionArchive*) target_section;
        SbsNodeArchive *archive_entry = sbs_section_archive_add_node(archive_section);
        archive_entry->base.condition = condition;
        sbs_section_archive_body_parse(parser, archive_section, archive_entry);
    }
    else if (target_type == SBS_TARGET_SHARED)
    {
        SbsSectionShared *shared_section = (SbsSectionShared*) target_section;
        SbsNodeShared *shared_entry = sbs_section_shared_add_node(shared_section);
        shared_entry->base.condition = condition;
        sbs_section_shared_body_parse(parser, shared_section, shared_entry);
    }
    else if (target_type == SBS_TARGET_EXECUTABLE)
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
