#include <stdio.h>
#include <fllib/Cstring.h>
#include <fllib/containers/Hashtable.h>
#include "file.h"
#include "action.h"
#include "configuration.h"
#include "environment.h"
#include "preset.h"
#include "target.h"
#include "toolchain.h"
#include "../io.h"
#include "../utils.h"

static unsigned long hash_variable_name(const FlByte *key)
{
    const SbsVariableInfo *var = (const SbsVariableInfo*) key;
    return fl_hashtable_hash_string((const FlByte*) var->fqn);
}

static bool variable_equals(const FlByte *val1, const FlByte *val2)
{
    const SbsVariableInfo *var1 = (const SbsVariableInfo*) val1;
    const SbsVariableInfo *var2 = (const SbsVariableInfo*) val2;

    return flm_cstring_equals(var1->fqn, var2->fqn);
}

static void map_init_variables(FlHashtable **variables)
{
    struct FlHashtableArgs new_args = {
        .hash_function = hash_variable_name, 
        .key_comparer = variable_equals,
        .value_cleaner = (FlContainerCleanupFn) sbs_node_variable_definition_free
    };
    
    *variables = fl_hashtable_new_args(new_args);
}

static void map_init_env(FlHashtable **envs)
{
    struct FlHashtableArgs new_args = {
        .hash_function = fl_hashtable_hash_string, 
        .key_allocator = fl_container_allocator_string,
        .key_comparer = fl_container_equals_string,
        .key_cleaner = fl_container_cleaner_pointer,
        .value_cleaner = (FlContainerCleanupFn) sbs_section_env_free
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
        .value_cleaner = (FlContainerCleanupFn) sbs_section_action_free
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
        .value_cleaner = (FlContainerCleanupFn) sbs_section_config_free
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
        .value_cleaner = (FlContainerCleanupFn) sbs_section_preset_free
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
        .value_cleaner = (FlContainerCleanupFn) sbs_section_target_free
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
        .value_cleaner = (FlContainerCleanupFn) sbs_section_toolchain_free
    };
    
    *toolchains = fl_hashtable_new_args(new_args);
}

SbsFile* sbs_file_new(const char *filename)
{
    SbsFile *file = fl_malloc(sizeof(SbsFile));

    file->filename = fl_cstring_dup(filename);
    map_init_action(&file->actions);
    map_init_config(&file->configurations);
    map_init_env(&file->envs);
    map_init_preset(&file->presets);
    map_init_target(&file->targets);
    map_init_toolchain(&file->toolchains);
    map_init_variables(&file->variables);

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
void sbs_file_free(SbsFile *file)
{
    if (file->filename)
        fl_cstring_free(file->filename);

    if (file->variables)
        fl_hashtable_free(file->variables);

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
