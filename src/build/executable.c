#include "executable.h"
#include "build.h"
#include "../common/common.h"
#include "../objects/configuration.h"
#include "../objects/toolchain.h"

#define SBS_DIR_SEPARATOR "/"

static char* build_output_filename(struct SbsBuild *build, const struct SbsConfigExecutable *executable, const char *output_dir, const char *output_name)
{
    // File
    const char *extension = executable->extension ? executable->extension : ".a";

    // Path
    char *output_filename = fl_cstring_dup(output_dir);
    if (output_filename[strlen(output_filename) - 1] != SBS_DIR_SEPARATOR[0])
        fl_cstring_append(&output_filename, SBS_DIR_SEPARATOR);
    fl_cstring_append(&output_filename, build->config->name);

    output_filename = fl_cstring_replace_realloc(output_filename, "\\", SBS_DIR_SEPARATOR);
    
    if (output_filename[strlen(output_filename) - 1] != SBS_DIR_SEPARATOR[0])
        fl_cstring_append(&output_filename, SBS_DIR_SEPARATOR);

    fl_io_dir_create_recursive(output_filename);

    // Create the fullname
    fl_cstring_append(fl_cstring_append(&output_filename, output_name), extension);

    return output_filename;
}

char** sbs_build_target_executable(struct SbsBuild *build)
{
    struct SbsTargetExecutable *target_executable = (struct SbsTargetExecutable*)build->target;
    const struct SbsConfigExecutable *config_executable = &build->config->executable;

    // Collect all the executable flags in the configuration hierarchy
    char *flags = fl_cstring_new(0);
    if (config_executable->flags)
    {
        for (size_t i = 0; i < fl_array_length(config_executable->flags); i++)
        {
            fl_cstring_append(&flags, config_executable->flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    char *executable_libraries = fl_cstring_new(0);
    if (target_executable->libraries)
    {
        for (size_t i = 0; i < fl_array_length(target_executable->libraries); i++)
        {
            struct SbsTargetLibrary *library = target_executable->libraries + i;
            if (library->path)
            {
                fl_cstring_append(&executable_libraries, build->toolchain->linker.lib_dir_flag);
                fl_cstring_append(&executable_libraries, library->path);
                fl_cstring_append(&executable_libraries, " ");
            }

            if (library->name)
            {
                fl_cstring_append(&executable_libraries, build->toolchain->linker.lib_flag);
                fl_cstring_append(&executable_libraries, library->name);
                fl_cstring_append(&executable_libraries, " ");
            }
        }
    }

    // Build the target's output filename
    char *output_filename = build_output_filename(build, config_executable, target_executable->output_dir, target_executable->output_name);

    // We add the filename to the output vector that will return the target's result
    char **output = fl_array_new(sizeof(char*), 1);
    output[0] = output_filename;

    // Get the timestamp of the output file (if it exists)
    unsigned long long executable_timestamp = 0;
    fl_io_file_get_modified_timestamp(output_filename, &executable_timestamp);

    // By default we assume we don't need to run the command (see below)
    bool needs_linkage = false;    

    // This vector will keep track of the objects that are needed to build the executable
    size_t n_objects = fl_array_length(target_executable->objects);
    FlVector *executable_objects = fl_vector_new(n_objects, fl_container_cleaner_pointer);    

    // We iterate through all the executable's objects where we can find two type of resources:
    //  1- An identifier that represents another target, we need to build that target and use its output
    //  2- An string that is the path to a file that can be directly included in the executable
    bool success = true;
    for (size_t i = 0; i < n_objects; i++)
    {
        if (target_executable->objects[i].type == SBS_IDENTIFIER)
        {
            // target_objects is an array of pointers to char allocated by the target
            struct SbsTarget *target = sbs_target_resolve(build->file, target_executable->objects[i].value, build->env->name, (const struct SbsTarget*) target_executable);

            char **target_objects = sbs_build_target(&(struct SbsBuild) {
                .executor = build->executor,
                .file = build->file,
                .env = build->env,
                .toolchain = build->toolchain,
                .target = target,
                .config = build->config
            });

            sbs_target_free(target);

            // Something odd happened if it is null, we need to leave with error
            if (target_objects == NULL)
            {
                success = false;
                break;
            }

            // Check every object file to see if it is newer than the last executable build's
            // modification timestamp. If there is just one object newer to the executable, we need to
            // run the build.
            for (size_t i=0; i < fl_array_length(target_objects); i++)
            {
                char *obj = target_objects[i];
                target_objects[i] = NULL;

                unsigned long long obj_timestamp = 0;
                if (!fl_io_file_get_modified_timestamp(obj, &obj_timestamp))
                    needs_linkage = true;

                if (executable_timestamp < obj_timestamp)
                    needs_linkage = true;

                fl_vector_add(executable_objects, obj);
            }

            fl_array_free(target_objects);
        }
        else
        {
            char *object_filename = fl_cstring_dup(target_executable->objects[i].value);

            object_filename = fl_cstring_replace_realloc(object_filename, "${SBS_ENV_NAME}", build->env->name);
            object_filename = fl_cstring_replace_realloc(object_filename, "${SBS_CONFIG_NAME}", build->config->name);
            object_filename = fl_cstring_replace_realloc(object_filename, "${SBS_TARGET_NAME}", build->target->name);
            object_filename = fl_cstring_replace_realloc(object_filename, "${SBS_TOOLCHAIN_NAME}", build->toolchain->name);

            // We also check here to see if the object pointed by the string is newer than the executable
            // in order to set the needs_linkage flag
            unsigned long long obj_timestamp;
            if (!fl_io_file_get_modified_timestamp(object_filename, &obj_timestamp))
                needs_linkage = true;

            if (executable_timestamp < obj_timestamp)
                needs_linkage = true;

            fl_vector_add(executable_objects, object_filename);
        }        
    }

    if (success && needs_linkage)
    {
        if (build->toolchain->linker.bin != NULL)
        {
            // Replace the special ${OUTPUT_FILE} variable in the flag
            char *executable_flags = fl_cstring_replace(flags, "${OUTPUT_FILE}", output_filename);

            // Build the compile command
            char *command = fl_cstring_vdup("%s %s ", build->toolchain->linker.bin, executable_flags);

            for (size_t i=0; i < fl_vector_length(executable_objects); i++)
                fl_cstring_append(fl_cstring_append(&command, " "), fl_vector_get(executable_objects, i));

            fl_cstring_append(fl_cstring_append(&command, " "), executable_libraries);

            // Exec
            success = sbs_executor_run_command(build->executor, command) && success;

            fl_cstring_free(command);
            fl_cstring_free(executable_flags);
        }
        else
        {
            success = false;
            fprintf(stdout, "Toolchain '%s' does not have an linker executable defined for environment '%s'", build->toolchain->name, build->env->name);
        }
    }
    else if (success)
    {
        fprintf(stdout, "File '%s' has not changed. Skipping target...\n", output_filename);
    }
    else
    {
        fprintf(stdout, "A dependency of '%s' is missing or not valid, could not build the output file.\n", output_filename);
    }

    fl_vector_free(executable_objects);
    fl_cstring_free(executable_libraries);
    fl_cstring_free(flags);

    if (!success)
    {
        fl_array_free_each(output, sbs_common_free_string);
        return NULL;
    }

    return output;
}
