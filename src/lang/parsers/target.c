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
static void parse_compile_body(SbsParser *parser, SbsSectionCompile *target_section, SbsNodeCompile *target);
static void parse_archive_body(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target);
static void parse_shared_body(SbsParser *parser, SbsSectionShared *target_section, SbsNodeShared *target);
static void parse_executable_body(SbsParser *parser, SbsSectionExecutable *target_section, SbsNodeExecutable *target);

static void parse_for_section(SbsParser *parser, SbsAbstractSectionTarget *target_section, SbsSectionTargetType target_type)
{
    const SbsToken *token = sbs_parser_peek(parser);

    // Parse the for declaration
    SbsSectionFor *for_clause = sbs_section_for_parse(parser);
    
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);

    if (target_type == SBS_TARGET_COMPILE)
    {
        SbsSectionCompile *compile_section = (SbsSectionCompile*) target_section;
        SbsNodeCompile *compile_entry = sbs_section_compile_add_node(compile_section);
        compile_entry->base.for_clause = for_clause;
        parse_compile_body(parser, compile_section, compile_entry);
    }
    else if (target_type == SBS_TARGET_ARCHIVE)
    {
        SbsSectionArchive *archive_section = (SbsSectionArchive*) target_section;
        SbsNodeArchive *archive_entry = sbs_section_archive_add_node(archive_section);
        archive_entry->base.for_clause = for_clause;
        parse_archive_body(parser, archive_section, archive_entry);
    }
    else if (target_type == SBS_TARGET_SHARED)
    {
        SbsSectionShared *shared_section = (SbsSectionShared*) target_section;
        SbsNodeShared *shared_entry = sbs_section_shared_add_node(shared_section);
        shared_entry->base.for_clause = for_clause;
        parse_shared_body(parser, shared_section, shared_entry);
    }
    else if (target_type == SBS_TARGET_EXECUTABLE)
    {
        SbsSectionExecutable *executable_section = (SbsSectionExecutable*) target_section;
        SbsNodeExecutable *executable_entry = sbs_section_executable_add_node(executable_section);
        executable_entry->base.for_clause = for_clause;
        parse_executable_body(parser, executable_section, executable_entry);
    }
    else
    {
        sbs_parser_error(token, "while parsing a for statement in a target section");
    }

    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);
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
            if (sbs_token_equals(token, "path"))
            {
                sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
                sbs_parser_consume(parser, SBS_TOKEN_COLON);
                library.path = sbs_parse_string(parser);
            }
            else if (sbs_token_equals(token, "name"))
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

static void parse_compile_body(SbsParser *parser, SbsSectionCompile *target_section, SbsNodeCompile *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);
        
        if (sbs_token_equals(token, "includes"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->includes = sbs_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "sources"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->sources = sbs_parse_string_array(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (sbs_token_equals(token, "defines"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_COMPILE);
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
    // Consume the 'compile' token
    sbs_parser_consume(parser, SBS_TOKEN_COMPILE);
    
    // Consume IDENTIFIER
    SbsSectionCompile *target_section = sbs_section_compile_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);
    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_compile_body(parser, target_section, sbs_section_compile_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}

static void parse_archive_body(SbsParser *parser, SbsSectionArchive *target_section, SbsNodeArchive *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (sbs_token_equals(token, "output_name"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_ARCHIVE);
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
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_ARCHIVE);
    
    // Consume IDENTIFIER
    SbsSectionArchive *target_section = sbs_section_archive_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_archive_body(parser, target_section, sbs_section_archive_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}

static void parse_shared_body(SbsParser *parser, SbsSectionShared *target_section, SbsNodeShared *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (sbs_token_equals(token, "output_name"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_SHARED);
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
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_SHARED);
    
    // Consume IDENTIFIER
    SbsSectionShared *target_section = sbs_section_shared_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_shared_body(parser, target_section, sbs_section_shared_add_node(target_section));
    sbs_parser_consume(parser, SBS_TOKEN_RBRACE);

    return (SbsAbstractSectionTarget*) target_section;
}

static void parse_executable_body(SbsParser *parser, SbsSectionExecutable *target_section, SbsNodeExecutable *target)
{
    while (sbs_parser_peek(parser)->type != SBS_TOKEN_RBRACE)
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (sbs_token_equals(token, "objects"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->objects = sbs_value_source_array_prase(parser);
        }
        else if (sbs_token_equals(token, "output_name"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->output_name = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "output_dir"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->base.output_dir = sbs_parse_string(parser);
        }
        else if (sbs_token_equals(token, "actions"))
        {
            target->base.actions = sbs_property_actions_parse(parser);
        }
        else if (sbs_token_equals(token, "libraries"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->libraries = parse_library_array(parser);
        }
        else if (sbs_token_equals(token, "defines"))
        {
            sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER);
            sbs_parser_consume(parser, SBS_TOKEN_COLON);
            target->defines = sbs_parse_string_array(parser);
        }
        else if (token->type == SBS_TOKEN_FOR)
        {
            parse_for_section(parser, (SbsAbstractSectionTarget*) target_section, SBS_TARGET_EXECUTABLE);
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
    // Consume 'target'
    sbs_parser_consume(parser, SBS_TOKEN_EXECUTABLE);
    
    // Consume IDENTIFIER
    SbsSectionExecutable *target_section = sbs_section_executable_new(&sbs_parser_consume(parser, SBS_TOKEN_IDENTIFIER)->value);

    sbs_parser_consume(parser, SBS_TOKEN_LBRACE);
    parse_executable_body(parser, target_section, sbs_section_executable_add_node(target_section));
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
