#include <stdio.h>
#include <fllib.h>

#include "types.h"
#include "executor.h"
#include "build.h"
#include "file.h"

#define SBS_DIR_SEPARATOR "/"

FlVector build_target_compile(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTargetCompile *compile, struct SbsConfiguration *config);
FlVector build_target_archive(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTargetArchive *archive, struct SbsConfiguration *config);
FlVector build_target(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTarget *target, struct SbsConfiguration *config);

void resolve_targets(const struct SbsFile *file, struct SbsTarget *target, FlVector targets)
{
    /*if (target->extends)
    {
        size_t extends = fl_array_length(target->extends);
        for (size_t i=0; i < extends; i++)
        {
            struct SbsTarget *parentTarget = fl_hashtable_get(file->targets, target->extends[i]);
            resolve_targets(file, parentTarget, targets);
        }
    }*/
    fl_vector_add(targets, target);
}

enum ActionType {
    ACTION_BEFORE,
    ACTION_AFTER
};

bool run_actions(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, struct SbsStringOrId *actions)
{
    if (!actions)
        return true;

    size_t nactions = fl_array_length(actions);
    for (size_t j=0; j < nactions; j++)
    {
        if (actions[j].type == SBS_IDENTIFIER)
        {
            struct SbsAction *action = fl_hashtable_get(file->actions, actions[j].value);

            if (!action || !action->commands)
                continue;

            size_t countOfCommands = fl_array_length(action->commands);
            for (size_t k=0; k < countOfCommands; k++)
            {
                struct SbsActionCommand command = action->commands[k];

                if (command.for_envs)
                {
                    bool should_run = false;
                    for (size_t i=0; i < fl_array_length(command.for_envs); i++)
                    {
                        if (flm_cstring_equals(command.for_envs[i], env->name))
                        {
                            should_run = true;
                            break;
                        }
                    }

                    if (!should_run)
                        continue;
                }

                if (command.commands)
                {
                    for (size_t i=0; i < fl_array_length(command.commands); i++)
                    {
                        if (!sbs_executor_run_command(executor, command.commands[i]))
                            return false;
                    }
                }
            }
        }
        else
        {
            if (!sbs_executor_run_command(executor, actions[j].value))
                return false;
        }
    }

    return true;
}

bool run_env_actions(SbsExecutor executor, const struct SbsFile *file, const struct SbsEnv *env, enum ActionType type)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? env->actions.before : env->actions.after;
    if (!run_actions(executor, file, env, actions))
        return false;

    return true;
}

bool run_target_actions(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsTarget *target, enum ActionType type)
{
    struct SbsStringOrId *actions = type == ACTION_BEFORE ? target->actions.before : target->actions.after;
    return run_actions(executor, file, env, actions);
}

FlVector build_target_compile(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTargetCompile *compile, struct SbsConfiguration *config)
{
    // Collect all the compile flags in the configuration hierarchy
    char *flags = fl_cstring_dup("");
    if (config->compile.flags)
    {
        for (size_t i = 0; i < fl_array_length(config->compile.flags); i++)
        {
            fl_cstring_append(&flags, config->compile.flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    // Glue all the includes together
    char *includes = fl_cstring_dup("");
    for (size_t i = 0; i < fl_array_length(compile->includes); i++)
    {
        fl_cstring_append(&includes, compile->include_flag);
        fl_cstring_append(&includes, compile->includes[i]);
        fl_cstring_append(&includes, " ");
    }

    // Keep track of objects
    FlVector objects = fl_vector_new(10, fl_container_cleaner_pointer);

    bool success = true;

    // Compile source objects
    for (size_t i = 0; success && i < fl_array_length(compile->sources); i++)
    {
        // Unify path separators for the source file
        char *source_file = fl_cstring_replace(compile->sources[i], "\\", SBS_DIR_SEPARATOR);

        unsigned long long source_timestamp;
        fl_io_file_get_modified_timestamp(source_file, &source_timestamp);

        // Split the source file path
        FlVector source_file_parts = fl_cstring_split_by(source_file, SBS_DIR_SEPARATOR);

        // Get the source filename
        char *filename = NULL;
        fl_vector_pop(source_file_parts, &filename);

        // If the user suplied an extension, use it
        const char *extension = config->compile.extension ? config->compile.extension : ".o";

        filename = fl_cstring_replace_realloc(filename, ".cpp", extension);
        filename = fl_cstring_replace_realloc(filename, ".c", extension);

        // Get the source file path to replicate it under the output path
        char *source_file_path = fl_cstring_join(source_file_parts, SBS_DIR_SEPARATOR);

        // Unify path separators for the output file
        char *output_file_fullpath = fl_cstring_dup(compile->base.output_dir);
        if (output_file_fullpath[strlen(output_file_fullpath) - 1] != SBS_DIR_SEPARATOR[0])
            fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);
        fl_cstring_append(&output_file_fullpath, config->name);

        output_file_fullpath = fl_cstring_replace_realloc(output_file_fullpath, "\\", SBS_DIR_SEPARATOR);
        
        if (output_file_fullpath[strlen(output_file_fullpath) - 1] != SBS_DIR_SEPARATOR[0])
            fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);

        fl_cstring_append(&output_file_fullpath, source_file_path);

        // Create the output directory
        fl_io_dir_create_recursive(output_file_fullpath);

        // Append the object's filename 
        char *object_file = fl_cstring_vdup("%s%s%s", output_file_fullpath, SBS_DIR_SEPARATOR, filename);

        // Add the object_file to the list of objects (no need to free object_file, it outlives this function)
        fl_vector_add(objects, object_file);

        // Check if the source file is older than the object file, in that case skip the compilation
        if (fl_io_file_exists(object_file))
        {
            unsigned long long object_timestamp;
            if (fl_io_file_get_modified_timestamp(object_file, &object_timestamp))
            {
                if (source_timestamp < object_timestamp)
                {
                    // printf("Source timestamp: %lld\n", source_timestamp);
                    // printf("Modified timestamp: %lld\n", object_timestamp);
                    printf("File '%s' has not changed. Skipping compilation...\n", source_file);
                    goto SKIP;
                }
            }
        }        

        // Replace the special ${source} and ${object} variables in the falgs
        char *unit_flags = fl_cstring_replace(flags, "${source}", source_file);
        unit_flags = fl_cstring_replace_realloc(unit_flags, "${object}", object_file);

        // Build the compile command
        char *command = fl_cstring_vdup("%s %s %s", toolchain->compiler, includes, unit_flags);

        // Exec
        success = sbs_executor_run_command(executor, command) && success;
        
        // clean
        fl_cstring_delete(command);
        fl_cstring_delete(unit_flags);
SKIP:
        fl_cstring_delete(output_file_fullpath);
        fl_cstring_delete(source_file_path);
        fl_cstring_delete(filename);
        fl_vector_delete(source_file_parts);
        fl_cstring_delete(source_file);
    }

    fl_cstring_delete(includes);
    fl_cstring_delete(flags);

    if (!success)
    {
        fl_vector_delete(objects);
        return NULL;
    }

    return objects;
}

FlVector build_target_archive(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTargetArchive *archive, struct SbsConfiguration *config)
{
    // Collect all the archive flags in the configuration hierarchy
    char *flags = fl_cstring_dup("");
    if (config->archive.flags)
    {
        for (size_t i = 0; i < fl_array_length(config->archive.flags); i++)
        {
            fl_cstring_append(&flags, config->archive.flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    // File
    const char *extension = config->compile.extension ? config->compile.extension : ".a";

    // Path
    char *output_file_fullpath = fl_cstring_dup(archive->base.output_dir);
    fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);
    fl_cstring_append(&output_file_fullpath, config->name);

    output_file_fullpath = fl_cstring_replace_realloc(output_file_fullpath, "\\", SBS_DIR_SEPARATOR);
    
    if (output_file_fullpath[strlen(output_file_fullpath) - 1] != SBS_DIR_SEPARATOR[0])
        fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);

    fl_io_dir_create_recursive(output_file_fullpath);

    // Create the fullname
    fl_cstring_append(fl_cstring_append(&output_file_fullpath, archive->output_name), extension);

    unsigned long long archive_timestamp = 0;
    fl_io_file_get_modified_timestamp(output_file_fullpath, &archive_timestamp);
    bool should_run = false;

    // Keep track of objects
    FlVector output = fl_vector_new(1, fl_container_cleaner_pointer);
    fl_vector_add(output, output_file_fullpath);

    FlVector objects = fl_vector_new(10, fl_container_cleaner_pointer);

    bool success = true;

    // Compile source objects
    for (size_t i = 0; i < fl_array_length(archive->objects); i++)
    {
        if (archive->objects[i].type == SBS_IDENTIFIER)
        {
            FlVector target_objects = build_target(executor, file, env, toolchain, fl_hashtable_get(file->targets, archive->objects[i].value), config);

            if (target_objects == NULL)
            {
                success = false;
                break;
            }

            while (fl_vector_length(target_objects) > 0)
            {
                char *obj = NULL;
                fl_vector_shift(target_objects, &obj);

                unsigned long long obj_timestamp = 0;
                if (!fl_io_file_get_modified_timestamp(obj, &obj_timestamp))
                    should_run = true;

                if (archive_timestamp < obj_timestamp)
                    should_run = true;

                fl_vector_add(objects, obj);
            }
            fl_vector_delete(target_objects);
        }
        else
        {
            unsigned long long obj_timestamp;
            if (!fl_io_file_get_modified_timestamp(archive->objects[i].value, &obj_timestamp))
                should_run = true;

            if (archive_timestamp < obj_timestamp)
                should_run = true;

            fl_vector_add(objects, archive->objects[i].value);
        }
        
        if (should_run)
        {
            // Replace the special ${source} and ${object} variables in the falgs
            char *archive_flags = fl_cstring_replace(flags, "${archive}", output_file_fullpath);

            // Build the compile command
            char *command = fl_cstring_vdup("%s %s", toolchain->archiver, archive_flags);

            for (size_t i=0; i < fl_vector_length(objects); i++)
                fl_cstring_append(fl_cstring_append(&command, " "), fl_vector_get(objects, i));

            // Exec
            success = sbs_executor_run_command(executor, command) && success;

            fl_cstring_delete(command);
            fl_cstring_delete(archive_flags);
        }
    }

    fl_vector_delete(objects);
    fl_cstring_delete(flags);

    if (!success)
    {
        fl_vector_delete(output);
        return NULL;
    }

    return output;
}

FlVector build_target(SbsExecutor executor, const struct SbsFile *file, struct SbsEnv *env, struct SbsToolchain *toolchain, struct SbsTarget *target, struct SbsConfiguration *config)
{
    if (target->output_dir && !fl_io_file_exists(target->output_dir))
    {
        if (!fl_io_dir_create_recursive(target->output_dir))
            printf("Failed to create directory %s\n", target->output_dir);
    }
    
    if (target->type == SBS_TARGET_COMPILE)
    {
        return build_target_compile(executor, file, env, toolchain, (struct SbsTargetCompile*)target, config);
    }
    else if (target->type == SBS_TARGET_ARCHIVE)
    {
        return build_target_archive(executor, file, env, toolchain, (struct SbsTargetArchive*)target, config);
    }
    else if (target->type == SBS_TARGET_EXECUTABLE)
    {
        struct SbsTargetExecutable *exe = (struct SbsTargetExecutable*)target;
    }

    return NULL;
}

void extend_fl_array(char ***dest, char** src)
{
    size_t config_flags_length = fl_array_length(src);
    size_t extended_flags_length = *dest ? fl_array_length(*dest) : 0;
    size_t flags_length = config_flags_length + extended_flags_length;

    *dest = *dest 
        ? fl_array_resize(*dest, flags_length)
        : fl_array_new(sizeof(char*), flags_length);

    for (size_t i = extended_flags_length; i < flags_length; i++)
        (*dest)[i] = src[i - extended_flags_length];
}

void find_config_ancestors(const struct SbsFile *file, const struct SbsConfiguration *config, FlList ancestors)
{
    if (!config->extends)
        return;

    size_t length = fl_array_length(config->extends);
    for (size_t i=0; i < length; i++)
        fl_list_prepend(ancestors, fl_hashtable_get(file->configurations, config->extends[i]));
    
    for (size_t i=0; i < length; i++)
        find_config_ancestors(file, fl_hashtable_get(file->configurations, config->extends[i]), ancestors);
}

void clean_config_inheritance(struct SbsConfiguration *extended_config)
{
    // char **for_toolchains;
    if (extended_config->for_toolchains)
        fl_array_delete(extended_config->for_toolchains);

    if (extended_config->compile.flags)
        fl_array_delete(extended_config->compile.flags);

    if (extended_config->compile.defines)
        fl_array_delete(extended_config->compile.defines);

    if (extended_config->archive.flags)
        fl_array_delete(extended_config->archive.flags);

    if (extended_config->shared.flags)
        fl_array_delete(extended_config->shared.flags);

    if (extended_config->executable.flags)
        fl_array_delete(extended_config->executable.flags);
}

bool resolve_config_inheritance(const struct SbsFile *file, const char *config_name, struct SbsConfiguration *extended_config)
{
    const struct SbsConfiguration *config = fl_hashtable_get(file->configurations, config_name);

    if (!config)
        return false;

    // Name will be always the targeted config
    extended_config->name = config->name;

    FlList ancestors = fl_list_new();

    // Using prepend we will keep the list ordered
    fl_list_prepend(ancestors, config);
    find_config_ancestors(file, config, ancestors);

    struct FlListNode *node = fl_list_head(ancestors);
    while (node)
    {
        const struct SbsConfiguration *ancestor = (const struct SbsConfiguration*)node->value;

        // char **for_toolchains;
        if (ancestor->for_toolchains)
            extend_fl_array(&(extended_config->for_toolchains), ancestor->for_toolchains);

        // struct SbsConfigCompile compile;
        if (ancestor->compile.extension)
            extended_config->compile.extension = ancestor->compile.extension;
        
        if (ancestor->compile.flags)
            extend_fl_array(&(extended_config->compile.flags), ancestor->compile.flags);

        if (ancestor->compile.defines)
            extend_fl_array(&(extended_config->compile.defines), ancestor->compile.defines);

        // struct SbsConfigArchive archive;
        if (ancestor->archive.extension)
            extended_config->archive.extension = ancestor->archive.extension;

        if (ancestor->archive.flags)
            extend_fl_array(&(extended_config->archive.flags), ancestor->archive.flags);

        // struct SbsConfigShared shared;
        if (ancestor->shared.extension)
            extended_config->shared.extension = ancestor->shared.extension;

        if (ancestor->shared.flags)
            extend_fl_array(&(extended_config->shared.flags), ancestor->shared.flags);

        // struct SbsConfigExecutable executable;
        if (ancestor->executable.extension)
            extended_config->executable.extension = ancestor->executable.extension;

        if (ancestor->executable.flags)
            extend_fl_array(&(extended_config->executable.flags), ancestor->executable.flags);

        node = node->next;
    }

    fl_list_delete(ancestors);

    return true;
}

void find_toolchain_ancestors(const struct SbsFile *file, const struct SbsToolchain *toolchain, FlList ancestors)
{
    if (!toolchain->extends)
        return;

    size_t length = fl_array_length(toolchain->extends);
    for (size_t i=0; i < length; i++)
        fl_list_prepend(ancestors, fl_hashtable_get(file->toolchains, toolchain->extends[i]));
    
    for (size_t i=0; i < length; i++)
        find_config_ancestors(file, fl_hashtable_get(file->toolchains, toolchain->extends[i]), ancestors);
}

void clean_toolchain_inheritance(struct SbsToolchain *extended_toolchain)
{
    // char **for_envs;
    if (extended_toolchain->for_envs)
        fl_array_delete(extended_toolchain->for_envs);
}

bool resolve_toolchain_inheritance(const struct SbsFile *file, const char *toolchain_name, struct SbsToolchain *extended_toolchain)
{
    const struct SbsToolchain *toolchain = fl_hashtable_get(file->toolchains, toolchain_name);

    if (!toolchain)
        return false;

    // Name will be always the targeted toolchain
    extended_toolchain->name = toolchain->name;

    FlList ancestors = fl_list_new();

    // Using prepend we will keep the list ordered
    fl_list_prepend(ancestors, toolchain);
    find_toolchain_ancestors(file, toolchain, ancestors);

    struct FlListNode *node = fl_list_head(ancestors);
    while (node)
    {
        const struct SbsToolchain *ancestor = (const struct SbsToolchain*)node->value;

        // char **for_toolchains;
        if (ancestor->for_envs)
            extend_fl_array(&(extended_toolchain->for_envs), ancestor->for_envs);

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

enum SbsResult sbs_build_run(const struct SbsFile *file, struct SbsBuildArguments args)
{
    struct SbsPreset *preset = NULL;
    if (args.preset)
    {
        preset = fl_hashtable_get(file->presets, args.preset);

        if (preset == NULL)
            return SBS_RES_INVALID_PRESET;
    }

    const char *env_name = preset && preset->env ? preset->env : args.env;
    const char *toolchain_name = preset && preset->toolchain ? preset->toolchain : args.toolchain;
    const char *target_name = preset && preset->target ? preset->target : args.target;
    const char *configuration_name = preset && preset->config ? preset->config : args.config;

    if (!env_name)
        return SBS_RES_MISSING_ENV_ARG;
    
    if (!toolchain_name)
        return SBS_RES_MISSING_TOOLCHAIN_ARG;

    if (!target_name)
        return SBS_RES_MISSING_TARGET_ARG;
    
    if (!configuration_name)
        return SBS_RES_MISSING_CONFIG_ARG;    

    if (!fl_hashtable_has_key(file->toolchains, toolchain_name))
        return SBS_RES_INVALID_TOOLCHAIN;
    
    if (!fl_hashtable_has_key(file->configurations, configuration_name))
        return SBS_RES_INVALID_CONFIG;

    // Get all the needed objects
    struct SbsEnv *env = fl_hashtable_get(file->envs, env_name);
    if (!env) 
        return SBS_RES_INVALID_ENV;

    struct SbsTarget *target = fl_hashtable_get(file->targets, target_name);
    if (!target) 
        return SBS_RES_INVALID_TARGET;
    
    defer_scope 
    {
        // Resolve the configuration to use
        struct SbsConfiguration extended_config = { 0 };

        if (!resolve_config_inheritance(file, configuration_name, &extended_config))
            defer_return SBS_RES_INVALID_CONFIG;

        defer_expression(clean_config_inheritance(&extended_config));

        // Resolve the toolchain to use
        struct SbsToolchain extended_toolchain = { 0 };

        if (!resolve_toolchain_inheritance(file, toolchain_name, &extended_toolchain))
            defer_return SBS_RES_INVALID_TOOLCHAIN;

        defer_expression(clean_toolchain_inheritance(&extended_toolchain));

        // Create the executor
        SbsExecutor executor = sbs_executor_create(env);
        defer_expression(sbs_executor_delete(executor));

        // 1) Run the "before" actions
        if (preset && preset->actions.before)
            if (!run_actions(executor, file, env, preset->actions.before))
                defer_return SBS_RES_ACTION_FAILED;

        if (!run_env_actions(executor, file, env, ACTION_BEFORE))
            defer_return SBS_RES_ACTION_FAILED;

        if (!run_target_actions(executor, file, env, target, ACTION_BEFORE))
            defer_return SBS_RES_ACTION_FAILED;

        // 2) Build the target
        FlVector vector = build_target(executor, file, env, &extended_toolchain, target, &extended_config);
        if (vector != NULL)
            defer_expression(fl_vector_delete(vector));

        if (preset && preset->actions.after)
            if (!run_actions(executor, file, env, preset->actions.after))
                defer_return SBS_RES_ACTION_FAILED;
    }

    return SBS_RES_OK;
}
