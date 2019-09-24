#include "compile.h"

#define SBS_DIR_SEPARATOR "/"

static char* build_object_filename(const struct SbsBuild *build, const struct SbsConfigCompile *config_compile, const char *source_file, const char *output_dir)
{
    // We need to build the object filename using the source filename for it
    //  ex: main.c -> main.o

    char *object_file = NULL;

    defer_scope {        
        FlVector source_file_parts = fl_cstring_split_by(source_file, SBS_DIR_SEPARATOR);
        defer_expression(fl_vector_delete(source_file_parts));
        
        char *filename = NULL;
        fl_vector_pop(source_file_parts, &filename);
        defer_expression(fl_cstring_delete(filename));

        // If the user supplied an extension, use it. If not take the default ".o"
        const char *extension = config_compile->extension ? config_compile->extension : ".o";

        filename = fl_cstring_replace_realloc(filename, ".cpp", extension);
        filename = fl_cstring_replace_realloc(filename, ".c", extension);

        // Get the source file path to replicate it under the output path
        //  ex: from the original filename "src/path/to/file.c" we get "src/path/to/"
        char *source_file_path = fl_cstring_join(source_file_parts, SBS_DIR_SEPARATOR);
        defer_expression(fl_cstring_delete(source_file_path));

        // Start building the output path
        char *output_file_fullpath = fl_cstring_dup(output_dir);
        defer_expression(fl_cstring_delete(output_file_fullpath));

        if (output_file_fullpath[strlen(output_file_fullpath) - 1] != SBS_DIR_SEPARATOR[0])
            fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);

        // Use the resources to create the directory structure
        fl_cstring_append(&output_file_fullpath, build->env->name);
        fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);
        fl_cstring_append(&output_file_fullpath, build->toolchain->name);
        fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);
        fl_cstring_append(&output_file_fullpath, build->config->name);
        fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);

        // Replace Windows-like directory separator
        output_file_fullpath = fl_cstring_replace_realloc(output_file_fullpath, "\\", SBS_DIR_SEPARATOR);
        
        if (output_file_fullpath[strlen(output_file_fullpath) - 1] != SBS_DIR_SEPARATOR[0])
            fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);

        // Append the source path structure
        fl_cstring_append(&output_file_fullpath, source_file_path);

        // Create the output directory
        fl_io_dir_create_recursive(output_file_fullpath);

        // Create the full path for the object file
        object_file = fl_cstring_vdup("%s%s%s", output_file_fullpath, SBS_DIR_SEPARATOR, filename);
    }

    return object_file;
}

char** sbs_build_compile(struct SbsBuild *build)
{
    struct SbsTargetCompile *target_compile = (struct SbsTargetCompile*)build->target;
    const struct SbsConfigCompile *config_compile = &build->config->compile;

    // Collect all the compile flags in the configuration hierarchy
    char *flags = fl_cstring_new(0);
    if (config_compile->flags)
    {
        for (size_t i = 0; i < fl_array_length(config_compile->flags); i++)
        {
            fl_cstring_append(&flags, config_compile->flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    // Glue all the includes together
    char *includes = fl_cstring_new(0);
    for (size_t i = 0; i < fl_array_length(target_compile->includes); i++)
    {
        fl_cstring_append(&includes, config_compile->include_dir_flag);
        fl_cstring_append(&includes, target_compile->includes[i]);
        fl_cstring_append(&includes, " ");
    }

    size_t n_sources = fl_array_length(target_compile->sources);
    // Keep track of the objects
    char **objects = fl_array_new(sizeof(char*), n_sources);

    // Compile source objects
    bool success = true;
    for (size_t i = 0; success && i < n_sources; i++)
    {
        // Unify path separators for the source file
        char *source_file = fl_cstring_replace(target_compile->sources[i], "\\", SBS_DIR_SEPARATOR);

        // Get the source's last modification timestamp
        unsigned long long source_timestamp;
        fl_io_file_get_modified_timestamp(source_file, &source_timestamp);

        // Get the object file path
        char *object_file = build_object_filename(build, config_compile, source_file, target_compile->output_dir);

        // Add the object_file to the list of objects (no need to free object_file, it outlives this function)
        objects[i] = object_file;

        // Check if the source file is older than the object file, in that case skip the compilation
        bool needs_compile = true;
        if (fl_io_file_exists(object_file))
        {
            unsigned long long object_timestamp;
            if (fl_io_file_get_modified_timestamp(object_file, &object_timestamp) && source_timestamp < object_timestamp)
            {
                fprintf(stdout, "File '%s' has not changed. Skipping compilation...\n", source_file);
                needs_compile = false;
            }
        }        

        if (needs_compile)
        {
            const char *compiler = sbs_toolchain_get_compiler(build->toolchain, build->env);

            if (compiler)
            {
                // Replace the special ${source} and ${object} variables in the falgs
                char *compilation_unit_flags = fl_cstring_replace(flags, "${source}", source_file);
                compilation_unit_flags = fl_cstring_replace_realloc(compilation_unit_flags, "${object}", object_file);

                // Build the compile command
                char *command = fl_cstring_vdup("%s %s %s", compiler, includes, compilation_unit_flags);

                // Exec
                success = sbs_executor_run_command(build->executor, command) && success;
                
                // clean
                fl_cstring_delete(command);
                fl_cstring_delete(compilation_unit_flags);
            }
            else
            {
                success = false;
                fprintf(stdout, "Toolchain '%s' does not have a compiler executable defined for environment '%s'", build->toolchain->name, build->env->name);
            }
        }

        fl_cstring_delete(source_file);
    }

    fl_cstring_delete(includes);
    fl_cstring_delete(flags);

    if (!success)
    {
        fl_array_delete_each(objects, sbs_common_free_string);
        return NULL;
    }

    return objects;
}
