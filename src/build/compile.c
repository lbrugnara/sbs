#include "compile.h"
#include "../common.h"
#include "../objects/configuration.h"

#define SBS_DIR_SEPARATOR "/"

// FIXME: Poor man's kind-of topological sort
static void resolve_c_file_dependencies(const char *target_file, const char ***resolved_files, char **include_folders, FlVector visited_files)
{
    char *content = fl_io_file_read_all_text(target_file);

    if (content == NULL)
        return;

    bool created_visited_files = false;
    if (!visited_files)
    {
        created_visited_files = true;
        visited_files = fl_vector_new(sizeof(char*), NULL);
    }

    // We use the current file's directory to process the includes
    FlVector target_file_parts = fl_cstring_split_by(target_file, SBS_DIR_SEPARATOR);
    fl_vector_pop(target_file_parts, NULL);
    const char *path = fl_cstring_join(target_file_parts, SBS_DIR_SEPARATOR);

    char *ptr = content;
    do {
        char *include = fl_cstring_find(ptr, "#include");

        if (include == NULL)
            break;

        include += 8; // skip #include

        const char *end = include + strlen(include);

        while (include != end && *include == ' ')
            include++;

        if (include != end && *include == '"')
        {
            // Skip the starting quote
            include++;
            char *quote_end = fl_cstring_find(include, "\"");
            if (!quote_end)
                break;
            *quote_end = '\0';
            char *file = fl_cstring_vdup("%s%s%s", path, SBS_DIR_SEPARATOR, include);

            bool need_to_visit = true;
            for (size_t i=0; need_to_visit && i < fl_vector_length(visited_files); i++)
            {
                if (flm_cstring_equals(file, fl_vector_get(visited_files, i)))
                    need_to_visit = false;
            }

            if (need_to_visit)
            {
                fl_vector_add(visited_files, file);
                resolve_c_file_dependencies(file, resolved_files, include_folders, visited_files);
            }

            *resolved_files = fl_array_append(*resolved_files, &file);

            include = ++quote_end;
        }
        else if (include_folders != NULL && include != end && *include == '<')
        {
            // Skip the starting angle
            include++;

            char *angle_end = fl_cstring_find(include, ">");
            if (!angle_end)
                break;

            *angle_end = '\0';

            for (size_t i=0; i < fl_array_length(include_folders); i++)
            {
                char *file = fl_cstring_vdup("%s%s%s", include_folders[i], SBS_DIR_SEPARATOR, include);

                if (fl_io_file_exists(file))
                {
                    bool need_to_visit = true;
                    for (size_t i=0; need_to_visit && i < fl_vector_length(visited_files); i++)
                    {
                        if (flm_cstring_equals(file, fl_vector_get(visited_files, i)))
                            need_to_visit = false;
                    }

                    if (need_to_visit)
                    {
                        fl_vector_add(visited_files, file);
                        resolve_c_file_dependencies(file, resolved_files, include_folders, visited_files);
                    }

                    *resolved_files = fl_array_append(*resolved_files, &file);
                }
                else
                {
                    fl_cstring_free(file);
                }
            }

            include = ++angle_end;
        }
        else
        {
            include++;
        }
        
        ptr = include;
    } while (ptr != NULL);

    if (created_visited_files)
        fl_vector_free(visited_files);

    fl_cstring_free(path);
    fl_vector_free(target_file_parts);
    fl_cstring_free(content);
}

const char** resolve_source_files(const char **target_sources)
{
    const char** source_files = fl_array_new(sizeof(const char*), 0);

    for (size_t i=0; i < fl_array_length(target_sources); i++)
    {
        if (fl_io_file_exists(target_sources[i]))
        {
            const char *source_file = fl_cstring_dup(target_sources[i]);
            source_files = fl_array_append(source_files, &source_file);
        }
        else
        {
            char **matched_files = fl_io_file_find(target_sources[i], SBS_DIR_SEPARATOR);

            if (matched_files)
            {
                source_files = fl_array_combine(source_files, matched_files);
                fl_array_free(matched_files);
            }
        }
    }

    return source_files;
}

static char* build_object_filename(const struct SbsBuild *build, const struct SbsConfigCompile *config_compile, const char *source_file, const char *output_dir)
{
    // We need to build the object filename using the source filename for it
    //  ex: main.c -> main.o

    char *object_file = NULL;

    FlVector source_file_parts = fl_cstring_split_by(source_file, SBS_DIR_SEPARATOR);

    char *filename = NULL;
    fl_vector_pop(source_file_parts, &filename);

    // If the user supplied an extension, use it. If not take the default ".o"
    const char *extension = config_compile->extension ? config_compile->extension : ".o";

    filename = fl_cstring_replace_realloc(filename, ".cpp", extension);
    filename = fl_cstring_replace_realloc(filename, ".c", extension);

    // Get the source file path to replicate it under the output path
    //  ex: from the original filename "src/path/to/file.c" we get "src/path/to/"
    char *source_file_path = fl_cstring_join(source_file_parts, SBS_DIR_SEPARATOR);

    // Start building the output path
    char *output_file_fullpath = fl_cstring_dup(output_dir);

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

    fl_cstring_free(output_file_fullpath);
    fl_cstring_free(source_file_path);
    fl_cstring_free(filename);
    fl_vector_free(source_file_parts);

    return object_file;
}

char** sbs_build_compile(struct SbsBuild *build)
{
    struct SbsTargetCompile *target_compile = (struct SbsTargetCompile*)build->target;

    if (!target_compile || !target_compile->sources)
        return NULL;

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
    for (size_t i = 0; target_compile->includes && i < fl_array_length(target_compile->includes); i++)
    {
        fl_cstring_append(&includes, build->toolchain->compiler.include_dir_flag);
        fl_cstring_append(&includes, target_compile->includes[i]);
        fl_cstring_append(&includes, " ");
    }

    // Glue all the defines together
    char *defines = fl_cstring_new(0);
    if (target_compile->defines)
    {
        for (size_t i = 0; i < fl_array_length(target_compile->defines); i++)
        {
            fl_cstring_append(&defines, build->toolchain->compiler.define_flag);
            fl_cstring_append(&defines, target_compile->defines[i]);
            fl_cstring_append(&defines, " ");
        }
    }

    const char **target_source_files = resolve_source_files((const char**)target_compile->sources);

    size_t n_sources = fl_array_length(target_source_files);

    // Keep track of the objects
    char **objects = fl_array_new(sizeof(char*), n_sources);

    // Compile source objects
    bool success = true;
    for (size_t i = 0; success && i < n_sources; i++)
    {
        // Unify path separators for the source file
        char *source_file = fl_cstring_replace(target_source_files[i], "\\", SBS_DIR_SEPARATOR);

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
                needs_compile = false;
                
            // The source file could be older than the object file, in that case we will check the header files included
            // in the source file to know if there are modifications made after the object file creation
            if (!needs_compile)
            {
                const char **deps = fl_array_new(sizeof(char*), 0);
                resolve_c_file_dependencies(source_file, &deps, target_compile->includes, NULL);

                for (size_t i=0; i < fl_array_length(deps); i++)
                {
                    const char *dep_file = deps[i];
            
                    if (fl_io_file_exists(dep_file))
                    {
                        unsigned long long dep_file_timestamp;
                        if (fl_io_file_get_modified_timestamp(dep_file, &dep_file_timestamp) && object_timestamp < dep_file_timestamp)
                        {
                            needs_compile = true;
                            break;
                        }
                    }
                }

                fl_array_free_each(deps, sbs_common_free_string);
            }
        }

        if (needs_compile)
        {
            if (build->toolchain->compiler.bin)
            {
                // Replace the special ${INPUT_FILE} and ${OUTPUT_FILE} variables in the falgs
                char *compilation_unit_flags = fl_cstring_replace(flags, "${INPUT_FILE}", source_file);
                compilation_unit_flags = fl_cstring_replace_realloc(compilation_unit_flags, "${OUTPUT_FILE}", object_file);

                // Build the compile command
                char *command = fl_cstring_vdup("%s %s %s %s", build->toolchain->compiler.bin, defines, includes, compilation_unit_flags);

                // Exec
                success = sbs_executor_run_command(build->executor, command) && success;
                
                // clean
                fl_cstring_free(command);
                fl_cstring_free(compilation_unit_flags);
            }
            else
            {
                success = false;
                fprintf(stdout, "Toolchain '%s' does not have a compiler executable defined for environment '%s'", build->toolchain->name, build->env->name);
            }
        }
        else
        {
            fprintf(stdout, "File '%s' has not changed. Skipping compilation...\n", source_file);
        }

        fl_cstring_free(source_file);
    }

    fl_array_free_each(target_source_files, sbs_common_free_string);
    fl_cstring_free(defines);
    fl_cstring_free(includes);
    fl_cstring_free(flags);

    if (!success)
    {
        fl_array_free_each(objects, sbs_common_free_string);
        return NULL;
    }

    return objects;
}
