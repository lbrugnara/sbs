#include <stdio.h>
#include <fllib.h>

#include "action.h"
#include "common.h"
#include "configuration.h"
#include "environment.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"
#include "lexer.h"
#include "parser.h"
#include "file.h"

#include "../common.h"


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
    char **keys = fl_hashtable_keys(src);

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
        fl_hashtable_remove(src, keys[i], false, false);
    }

    // It is ok to free the keys because of 2 reasons:
    // 1- We removed the entries from *src* WITHOUT cleaning the resources (fl_hashtable_remove with clean_key = false as 3rd parameter)
    // 2- The "dest" hashtable uses the fl_container_allocator_string therefore it creates copies of the keys on fl_hashtable_add
    fl_array_free_each(keys, sbs_common_free_string);
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
static void merge_into_file(struct SbsFile *dest_file, struct SbsFile *src_file)
{
    if (dest_file == NULL || src_file == NULL)
        return;

    merge_hashtable(dest_file->actions, src_file->actions);
    merge_hashtable(dest_file->configurations, src_file->configurations);
    merge_hashtable(dest_file->envs, src_file->envs);
    merge_hashtable(dest_file->presets, src_file->presets);
    merge_hashtable(dest_file->targets, src_file->targets);
    merge_hashtable(dest_file->toolchains, src_file->toolchains);
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
static bool parse_include_statement(struct SbsParser *parser, struct SbsFile *file)
{
    sbs_parser_consume(parser, SBS_TOKEN_INCLUDE);
    char **includes = sbs_common_parse_string_array(parser);

    // Empty array, leave. TODO: We should warning on verbosity output
    if (!includes)
        return true;

    // We need to get the current working directory based on the main SbsFile
    char *current_dir = NULL;
    {
        // Remove the filename to get the current directory
        FlVector *parts = fl_cstring_split_by(file->filename, "/");
        fl_vector_pop(parts, NULL);

        if (fl_vector_length(parts) > 0)
        {
            current_dir = fl_cstring_join(parts, "/");
            fl_cstring_append(&current_dir, "/");
        }
        else
        {
            current_dir = fl_cstring_dup("./");
        }

        fl_vector_free(parts);
    }

    // Iterate over the included files to parse them
    for (size_t i=0; i < fl_array_length(includes); i++)
    {
        // Get the filename
        char *filename = fl_cstring_vdup("%s%s", current_dir, includes[i]);

        // Parse the included file
         struct SbsFile *included_file = sbs_file_parse(filename);

        // Merge the included build file into our main build file
        merge_into_file(file, included_file);

        // Release the included file memory
        sbs_file_free(included_file);

        // Release the filename
        fl_cstring_free(filename);
    }

    // Release the memory used by our tmp variables
    fl_cstring_free(current_dir);
    fl_array_free_each(includes, sbs_common_free_string);

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
static bool parse_file(struct SbsParser *parser, struct SbsFile *file)
{
    bool success = true;
    while (success && sbs_parser_has_input(parser))
    {
        const struct SbsToken *token = sbs_parser_peek(parser);

        if (token->type == SBS_TOKEN_INCLUDE)
        {
            if (!parse_include_statement(parser, file))
            {
                success = false;
                break;
            }
        }
        else if (token->type == SBS_TOKEN_ENV)
        {
            const struct SbsEnvSection *env = sbs_env_section_parse(parser);
            if (fl_hashtable_has_key(file->envs, env->name))
            {
                printf("Env %s cannot be redefined\n", env->name);
                sbs_env_section_free((struct SbsEnvSection*)env);
                success = false;
                break;
            }
            fl_hashtable_add(file->envs, env->name, env);
        }
        else if (token->type == SBS_TOKEN_COMPILE || token->type == SBS_TOKEN_ARCHIVE || token->type == SBS_TOKEN_SHARED || token->type == SBS_TOKEN_EXECUTABLE)
        {
            const struct SbsTargetSection *target = sbs_target_section_parse(parser);
            if (fl_hashtable_has_key(file->targets, target->name))
            {
                printf("Target %s cannot be redefined\n", target->name);
                sbs_target_section_free((struct SbsTargetSection*)target);
                success = false;
                break;
            }
            fl_hashtable_add(file->targets, target->name, target);
        }
        else if (token->type == SBS_TOKEN_TOOLCHAIN)
        {
            const struct SbsToolchainSection *toolchain = sbs_toolchain_section_parse(parser);
            if (fl_hashtable_has_key(file->toolchains, toolchain->name))
            {
                printf("Toolchain %s cannot be redefined\n", toolchain->name);
                sbs_toolchain_section_free((struct SbsToolchainSection*)toolchain);
                success = false;
                break;
            }
            fl_hashtable_add(file->toolchains, toolchain->name, toolchain);
        }
        else if (token->type == SBS_TOKEN_CONFIG)
        {
            const struct SbsConfigSection *configuration = sbs_config_section_parse(parser);
            if (fl_hashtable_has_key(file->configurations, configuration->name))
            {
                printf("Configuration %s cannot be redefined\n", configuration->name);
                sbs_config_section_free((struct SbsConfigSection*)configuration);
                success = false;
                break;
            }
            fl_hashtable_add(file->configurations, configuration->name, configuration);
        }
        else if (token->type == SBS_TOKEN_ACTION)
        {
            const struct SbsActionSection *action = sbs_action_section_parse(parser);
            if (fl_hashtable_has_key(file->actions, action->name))
            {
                printf("Action %s cannot be redefined\n", action->name);
                sbs_action_section_free((struct SbsActionSection*)action);
                success = false;
                break;
            }
            fl_hashtable_add(file->actions, action->name, action);
        }
        else if (token->type == SBS_TOKEN_PRESET)
        {
            const struct SbsPresetSection *preset = sbs_preset_section_parse(parser);
            if (fl_hashtable_has_key(file->presets, preset->name))
            {
                printf("Preset %s cannot be redefined\n", preset->name);
                sbs_preset_section_free((struct SbsPresetSection*)preset);
                success = false;
                break;
            }
            fl_hashtable_add(file->presets, preset->name, preset);
        }
        else
        {
            sbs_parser_error(token, "while parsing the file");
        }
    }

    return success;
}

static void map_init_env(FlHashtable **envs)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_env_section_free
    };
    
    *envs = fl_hashtable_new_args(new_args);
}

static void map_init_action(FlHashtable **actions)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_action_section_free
    };
    
    *actions = fl_hashtable_new_args(new_args);
}

static void map_init_config(FlHashtable **config_map)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_config_section_free
    };
    
    *config_map = fl_hashtable_new_args(new_args);
}

static void map_init_preset(FlHashtable **presets)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_preset_section_free
    };
    
    *presets = fl_hashtable_new_args(new_args);
}

static void map_init_target(FlHashtable **targets)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_target_section_free
    };
    
    *targets = fl_hashtable_new_args(new_args);
}

static void map_init_toolchain(FlHashtable **toolchains)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (void(*)(void*))sbs_toolchain_section_free
    };
    
    *toolchains = fl_hashtable_new_args(new_args);
}

/*
 * Function: sbs_file_parse
 *  This function reads and parses all the build file's content
 *
 * Parameters:
 *  filename - Build file path
 *
 * Returns:
 *  struct SbsFile* - Parsed build file or NULL on error
 *
 */
struct SbsFile* sbs_file_parse(const char *filename)
{
    const char *source = (const char*)fl_io_file_read_all_text(filename);

    if (source == NULL)
        return NULL;

    // Create the SbsFile object
    struct SbsFile *file = fl_malloc(sizeof(struct SbsFile));
    file->filename = fl_cstring_replace(filename, "\\", "/");
    map_init_action(&file->actions);
    map_init_config(&file->configurations);
    map_init_env(&file->envs);
    map_init_preset(&file->presets);
    map_init_target(&file->targets);
    map_init_toolchain(&file->toolchains);

    struct SbsLexer lexer = sbs_lexer_new(source, strlen(source));
    struct SbsParser parser = {
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

    fl_array_free((struct SbsToken*)parser.tokens);
    // The parse function dups the strings, so it is safe to release the memory
    // used by the source
    fl_cstring_free((char*)source);

    return file;
}

/*
 * Function: sbs_file_free
 *  Releases the memory used by the *file* object.
 *
 * Parameters:
 *  file - Object to release its memory
 *
 * Returns:
 *  void - This function does not return a value
 */
void sbs_file_free(struct SbsFile *file)
{
    if (file->filename)
        fl_cstring_free(file->filename);

    // Hashtables should deallocate its key-value objects'
    // memory. Check the init_file function in the parser.c file.
    if (file->presets)
        fl_hashtable_free(file->presets);

    if (file->envs)
        fl_hashtable_free(file->envs);

    if (file->targets)
        fl_hashtable_free(file->targets);

    if (file->toolchains)
        fl_hashtable_free(file->toolchains);

    if (file->configurations)
        fl_hashtable_free(file->configurations);

    if (file->actions)
        fl_hashtable_free(file->actions);

    fl_free(file);
}
