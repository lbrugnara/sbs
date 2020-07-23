#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include <fllib/Array.h>
#include "target.h"
#include "action.h"
#include "source.h"
#include "helpers.h"
#include "parser.h"
#include "for.h"
#include "../../utils.h"

static void parse_for_section(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type);
static void parse_compile_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionCompile *target);
static void parse_archive_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionArchive *target);
static void parse_shared_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionShared *target);
static void parse_executable_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionExecutable *target);

static void parse_for_section(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type)
{
    const SbsToken *token = sbs_parser_peek(parser);

    // Parse the for declaration
    SbsSectionFor *for_clause = sbs_section_for_parse(parser);
    
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    // Parse the configuration info
    SbsSectionTarget *target_entry = NULL;

    if (target_type == SBS_TARGET_COMPILE)
    {
        target_entry = fl_malloc(sizeof(SbsSectionCompile));
        target_section->entries = fl_array_append(target_section->entries, &target_entry);
        parse_compile_body(parser, target_section, (SbsSectionCompile*)target_entry);
    }
    else if (target_type == SBS_TARGET_ARCHIVE)
    {
        target_entry = fl_malloc(sizeof(SbsSectionArchive));
        target_section->entries = fl_array_append(target_section->entries, &target_entry);
        parse_archive_body(parser, target_section, (SbsSectionArchive*)target_entry);
    }
    else if (target_type == SBS_TARGET_SHARED)
    {
        target_entry = fl_malloc(sizeof(SbsSectionShared));
        target_section->entries = fl_array_append(target_section->entries, &target_entry);
        parse_shared_body(parser, target_section, (SbsSectionShared*)target_entry);
    }
    else if (target_type == SBS_TARGET_EXECUTABLE)
    {
        target_entry = fl_malloc(sizeof(SbsSectionExecutable));
        target_section->entries = fl_array_append(target_section->entries, &target_entry);
        parse_executable_body(parser, target_section, (SbsSectionExecutable*)target_entry);
    }
    else
    {
        sbs_parser_error(token, "while parsing a for statement in a target section");
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    target_entry->for_clause = for_clause;
}

/*
 * Function: parse_library_array
 *  Returns an array of strings or identifiers
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  char** - Parsed array of strings or identifiers
 *
 */
static SbsPropertyLibrary* parse_library_array(SbsParser *parser)
{
    sbs_parser_consume(parser, SBS_TOKEN_LBRACKET);

    SbsPropertyLibrary *libraries = fl_array_new(sizeof(SbsPropertyLibrary), 0);

    const SbsToken *token;
    while ((token = sbs_parser_peek(parser))->type != SBS_TOKEN_RBRACKET)
    {
        sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

        SbsPropertyLibrary library = { 0 };

        while ((token = sbs_parser_peek(parser))->type != SBS_TOKEN_RBRACE)
        {
            if (fl_slice_equals_sequence(&token->value, (FlByte*)"path", 4))
            {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.path = sbs_parse_string(parser);
            }
            else if (fl_slice_equals_sequence(&token->value, (FlByte*)"name", 4))
            {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.name = sbs_parse_string(parser);
            }
            else
            {
                sbs_parser_error(token, "while parsing a target library node");
            }
        }

        sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

        libraries = fl_array_append(libraries, &library);
        
        sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACKET);

    return libraries;
}

static void parse_compile_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionCompile *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        
        if (fl_slice_equals_sequence(&token->value, (FlByte*)"includes", 8))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->includes = sbs_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"sources", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->sources = sbs_parse_string_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"defines", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
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
 *  SbsAbstractSectionTarget* - Parsed *compile* target
 *
 */
SbsAbstractSectionTarget* sbs_target_parse_compile(SbsParser *parser)
{
    SbsAbstractSectionTarget *target_section = fl_malloc(sizeof(SbsAbstractSectionTarget));
    target_section->type = SBS_TARGET_COMPILE;
    target_section->entries = fl_array_new(sizeof(SbsSectionTarget*), 0);

    // Consume the 'compile' token
    sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*) identifier->value.sequence, identifier->value.length);

    SbsSectionCompile *target_entry = fl_malloc(sizeof(SbsSectionCompile));
    target_section->entries = fl_array_append(target_section->entries, &target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);    
    parse_compile_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}

static void parse_archive_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionArchive *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        //const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_property_actions_parse(parser);
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
 *  SbsAbstractSectionTarget* - Parsed *archive* target
 *
 */
SbsAbstractSectionTarget* sbs_target_parse_archive(SbsParser *parser)
{
    SbsAbstractSectionTarget *target_section = fl_malloc(sizeof(SbsAbstractSectionTarget));
    target_section->type = SBS_TARGET_ARCHIVE;
    target_section->entries = fl_array_new(sizeof(SbsSectionTarget*), 0);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*)identifier->value.sequence, identifier->value.length);

    SbsSectionArchive *target_entry = fl_malloc(sizeof(SbsSectionArchive));
    target_section->entries = fl_array_append(target_section->entries, &target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_archive_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*)target_section;
}

static void parse_shared_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionShared *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        //const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_property_actions_parse(parser);
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
 *  SbsAbstractSectionTarget* - Parsed *shared* target
 *
 */
SbsAbstractSectionTarget* sbs_target_parse_shared(SbsParser *parser)
{
    SbsAbstractSectionTarget *target_section = fl_malloc(sizeof(SbsAbstractSectionTarget));
    target_section->type = SBS_TARGET_SHARED;
    target_section->entries = fl_array_new(sizeof(SbsSectionTarget*), 0);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*) identifier->value.sequence, identifier->value.length);

    SbsSectionShared *target_entry = fl_malloc(sizeof(SbsSectionShared));
    target_section->entries = fl_array_append(target_section->entries, &target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_shared_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*)target_section;
}

static void parse_executable_body(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionExecutable *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        //const SbsToken *token = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

        if (fl_slice_equals_sequence(&token->value, (FlByte*)"objects", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_name", 11))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"output_dir", 10))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"actions", 7))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"libraries", 9))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->libraries = parse_library_array(parser);
        }
        else if (fl_slice_equals_sequence(&token->value, (FlByte*)"defines", 7))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
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
 *  SbsAbstractSectionTarget* - Parsed *executable* target
 *
 */
SbsAbstractSectionTarget* sbs_target_parse_executable(SbsParser *parser)
{
    SbsAbstractSectionTarget *target_section = fl_malloc(sizeof(SbsAbstractSectionTarget));
    target_section->type = SBS_TARGET_EXECUTABLE;
    target_section->entries = fl_array_new(sizeof(SbsSectionTarget*), 0);

    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    const SbsToken *identifier = sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);

    target_section->name = fl_cstring_dup_n((const char*) identifier->value.sequence, identifier->value.length);

    SbsSectionExecutable *target_entry = fl_malloc(sizeof(SbsSectionExecutable));
    target_section->entries = fl_array_append(target_section->entries, &target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_executable_body(parser, target_section, target_entry);
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}

/*
 * Function: sbs_section_target_parse
 *  The root function to parse a *target* declaration. It just calls the
 *  specific function based on the token
 *
 * Parameters:
 *  parser - Parser object
 *
 * Returns:
 *  SbsAbstractSectionTarget* - The parsed *target*
 *
 */
SbsAbstractSectionTarget* sbs_section_target_parse(SbsParser *parser)
{
    const SbsToken *token = sbs_parser_peek(parser);

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
