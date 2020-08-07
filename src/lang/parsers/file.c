#include <stdio.h>
#include <fllib/Mem.h>
#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>

#include "file.h"
#include "cstring.h"
#include "action.h"
#include "helpers.h"
#include "configuration.h"
#include "environment.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"
#include "lexer.h"
#include "parser.h"
#include "variable.h"

#include "../../io.h"
#include "../../utils.h"


/*
 * Function: merge_hashtable
 *  Copies all the keys and values from the source hashtable into the
 *  destination hashtable
 *
 * Parameters:
 *  dest - Destination hashtable
 *  src - Source hashtable
 *
 * Returns:
 *  void - This function does not return a value
 */
static void merge_hashtable(FlHashtable *dest, FlHashtable *src)
{
    // We just get the source hashtable's keys
    void **keys = fl_hashtable_keys(src);

    if (!keys)
        return;

    // Iterate over the keys copying the entries
    for (size_t i=0; i < fl_array_length(keys); i++)
    {
        // The key will be copied into the *dest* hashtable because it must have a key allocator 
        // (fl_container_allocator_string). The value is passed as a pointer, so it remains equals 
        // to the entry in the *src* hashtable.
        fl_hashtable_add(dest, keys[i], fl_hashtable_get(src, keys[i]));

        // We pass *false* to prevent the source hashtable of cleaning the memory of our
        // values (and keys), because as mentioned above the values are the same pointers
        fl_hashtable_remove(src, keys[i], true, false);
    }

    // It is ok to free the keys because of 2 reasons:
    // 1- We removed the entries from *src* WITHOUT cleaning the resources (fl_hashtable_remove with clean_key = false as 3rd parameter)
    // 2- The "dest" hashtable uses the fl_container_allocator_string therefore it creates copies of the keys on fl_hashtable_add
    //fl_array_free_each_pointer(keys, (FlArrayFreeElementFunc) fl_cstring_free);
    fl_array_free(keys);
}

/*
 * Function: merge_into_file
 *  Merge all the source build file objects with the destination build file.
 *
 * Parameters:
 *  dest_file - Build file to include the new resources
 *  src_file - Build file to get all its resources merged into *dest_file*
 *
 * Returns:
 *  void - This function does not return a value
 *
 */
static void merge_into_file(SbsFile *dest_file, SbsFile *src_file)
{
    if (dest_file == NULL || src_file == NULL)
        return;

    merge_hashtable(dest_file->actions, src_file->actions);
    merge_hashtable(dest_file->configurations, src_file->configurations);
    merge_hashtable(dest_file->envs, src_file->envs);
    merge_hashtable(dest_file->presets, src_file->presets);
    merge_hashtable(dest_file->targets, src_file->targets);
    merge_hashtable(dest_file->toolchains, src_file->toolchains);
    merge_hashtable(dest_file->variables, src_file->variables);
}

/*
 * Function: parse_include_statement
 *  The include statement is an array of strings that point to other sbs files
 *  to parse and include to the current build file.
 *
 * Parameters:
 *  parser - Parser object
 *  file -  Build file being filled with the parser's output
 *
 * Returns:
 *  *bool* - *true* on success, otherwise *false*
 *
 */
static bool parse_include_statement(SbsParser *parser, SbsFile *file)
{
    sbs_parser_consume(parser, SBS_TOKEN_INCLUDE);
    char **includes = sbs_cstring_array_parse(parser);

    // Empty array, leave. TODO: We should warning on verbosity output
    if (!includes)
        return true;

    // We need to get the current working directory based on the main SbsFile
    char *current_dir = NULL;
    {
        // Remove the filename to get the current directory
        FlVector *parts = fl_cstring_split_by(file->filename, FL_IO_DIR_SEPARATOR);
        fl_vector_pop(parts, NULL);

        if (fl_vector_length(parts) > 0)
        {
            current_dir = fl_cstring_join(parts, FL_IO_DIR_SEPARATOR);
            fl_cstring_append(&current_dir, FL_IO_DIR_SEPARATOR);
        }
        else
        {
            current_dir = fl_cstring_vdup(".%s", FL_IO_DIR_SEPARATOR);
        }

        fl_vector_free(parts);
    }

    // Iterate over the included files to parse them
    for (size_t i=0; i < fl_array_length(includes); i++)
    {
        const char *include_filename = sbs_io_to_host_path(sbs_host_os(), includes[i]);
        // Get the filename
        char *filename = fl_cstring_vdup("%s%s", current_dir, include_filename);

        // Parse the included file
         SbsFile *included_file = sbs_file_parse(filename);

        if (included_file == NULL)
        {
            // TODO: Error handling here?
            continue;
        }

        // Merge the included build file into our main build file
        merge_into_file(file, included_file);

        // Release the included file memory
        sbs_file_free(included_file);

        // Release the filename
        fl_cstring_free(filename);
        fl_cstring_free(include_filename);
    }

    // Release the memory used by our tmp variables
    fl_cstring_free(current_dir);
    fl_array_free_each_pointer(includes, (FlArrayFreeElementFunc) fl_cstring_free);

    return true;
}

/*
 * Function: parse_file
 *  Processes the tokens returned by the lexer and fills the *file* object.
 *
 * Parameters:
 *  lexer - Lexer to ask for tokens
 *  file - Build file object to keep track of the parsed resources
 *
 * Returns:
 *  *bool* - If the parsing process is successful, this function returns *true*. Otherwise returns *false*.
 *
 */
static bool parse_file(SbsParser *parser, SbsFile *file)
{
    bool success = true;
    while (success && sbs_parser_has_input(parser))
    {
        const SbsToken *token = sbs_parser_peek(parser);

        if (token->type == SBS_TOKEN_INCLUDE)
        {
            if (!parse_include_statement(parser, file))
            {
                success = false;
                break;
            }
        }
        else if (token->type == SBS_TOKEN_VARIABLE)
        {
            SbsNodeVariableDefinition *var_def = sbs_parse_variable_definition(parser);
            if (fl_hashtable_has_key(file->variables, var_def->name))
            {
                printf("Variable %s cannot be redefined\n", var_def->name->fqn);
                sbs_node_variable_definition_free((SbsNodeVariableDefinition*) var_def);
                success = false;
                break;
            }
            fl_hashtable_add(file->variables, var_def->name, var_def);
            sbs_parser_consume_if(parser, SBS_TOKEN_COMMA);
        }
        else if (token->type == SBS_TOKEN_ENV)
        {
            const SbsSectionEnv *env = sbs_section_env_parse(parser);
            if (fl_hashtable_has_key(file->envs, env->name))
            {
                printf("Env %s cannot be redefined\n", env->name);
                sbs_section_env_free((SbsSectionEnv*)env);
                success = false;
                break;
            }
            fl_hashtable_add(file->envs, env->name, env);
        }
        else if (token->type == SBS_TOKEN_COMPILE || token->type == SBS_TOKEN_ARCHIVE || token->type == SBS_TOKEN_SHARED || token->type == SBS_TOKEN_EXECUTABLE)
        {
            const SbsAbstractSectionTarget *target = sbs_section_target_parse(parser);
            if (fl_hashtable_has_key(file->targets, target->name))
            {
                printf("Target %s cannot be redefined\n", target->name);
                sbs_section_target_free((SbsAbstractSectionTarget*)target);
                success = false;
                break;
            }
            fl_hashtable_add(file->targets, target->name, target);
        }
        else if (token->type == SBS_TOKEN_TOOLCHAIN)
        {
            const SbsSectionToolchain *toolchain = sbs_section_toolchain_parse(parser);
            if (fl_hashtable_has_key(file->toolchains, toolchain->name))
            {
                printf("Toolchain %s cannot be redefined\n", toolchain->name);
                sbs_section_toolchain_free((SbsSectionToolchain*)toolchain);
                success = false;
                break;
            }
            fl_hashtable_add(file->toolchains, toolchain->name, toolchain);
        }
        else if (token->type == SBS_TOKEN_CONFIG)
        {
            const SbsSectionConfig *configuration = sbs_section_config_parse(parser);
            if (fl_hashtable_has_key(file->configurations, configuration->name))
            {
                printf("Configuration %s cannot be redefined\n", configuration->name);
                sbs_section_config_free((SbsSectionConfig*)configuration);
                success = false;
                break;
            }
            fl_hashtable_add(file->configurations, configuration->name, configuration);
        }
        else if (token->type == SBS_TOKEN_ACTION)
        {
            const SbsSectionAction *action = sbs_section_action_parse(parser);
            if (fl_hashtable_has_key(file->actions, action->name))
            {
                printf("Action %s cannot be redefined\n", action->name);
                sbs_section_action_free((SbsSectionAction*)action);
                success = false;
                break;
            }
            fl_hashtable_add(file->actions, action->name, action);
        }
        else if (token->type == SBS_TOKEN_PRESET)
        {
            const SbsSectionPreset *preset = sbs_section_preset_parse(parser);
            if (fl_hashtable_has_key(file->presets, preset->name))
            {
                printf("Preset %s cannot be redefined\n", preset->name);
                sbs_section_preset_free((SbsSectionPreset*)preset);
                success = false;
                break;
            }
            fl_hashtable_add(file->presets, preset->name, preset);
        }
        else
        {
            sbs_parser_error(parser, token, "while parsing the file");
        }
    }

    return success && !parser->has_errors;
}

/*
 * Function: sbs_file_parse
 *  This function reads and parses all the build file's content
 *
 * Parameters:
 *  filename - Build file path
 *
 * Returns:
 *  SbsFile* - Parsed build file or NULL on error
 *
 */
SbsFile* sbs_file_parse(const char *filename)
{
    const char *source = (const char*) fl_io_file_read_all_text(filename);

    if (source == NULL)
        return NULL;

    // Create the SbsFile object
    SbsFile *file = sbs_file_new(filename);

    SbsLexer lexer = sbs_lexer_new(source, strlen(source));
    SbsParser parser = {
        .filename = filename,
        // TODO: We can use a buffer here and use the sbs_lexer_next function
        .tokens = sbs_lexer_tokenize(&lexer),
        .index = 0,
        .length = fl_array_length((FlArray*) parser.tokens)
    };
    
    if (!parse_file(&parser, file))
    {
        fl_free(file);
        file = NULL;
    }

    fl_array_free((SbsToken*)parser.tokens);
    // The parse function dups the strings, so it is safe to release the memory
    // used by the source
    fl_cstring_free((char*)source);

    return file;
}
