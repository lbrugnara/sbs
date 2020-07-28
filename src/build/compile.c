#include <fllib/Array.h>
#include <fllib/Cstring.h>
#include <fllib/IO.h>
#include <fllib/containers/Vector.h>
#include "compile.h" 
#include "../io.h"
#include "../utils.h"
#include "../runtime/configuration.h"

// FIXME: Poor man's kind-of topological sort
static void resolve_c_file_dependencies(SbsBuild *build, const char *target_file, char ***resolved_files, char **include_folders, FlVector *visited_files)
{
    char *content = fl_io_file_read_all_text(target_file);

    if (content == NULL)
        return;

    bool created_visited_files = false;
    if (!visited_files)
    {
        created_visited_files = true;
        visited_files = fl_vector_new();
    }

    // We use the current file's directory to process the includes
    FlVector *target_file_parts = fl_cstring_split_by(target_file, (char[]){ build->context->host->dir_separator, '\0' });
    fl_vector_pop(target_file_parts, NULL);
    const char *path = fl_cstring_join(target_file_parts, (char[]){ build->context->host->dir_separator, '\0' });

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
            char *quote_end = include;

            while (true)
            {
                if (*quote_end == '\n' || *quote_end == '"')
                    break;
                
                quote_end++;
            }
            
            if (!quote_end || *quote_end != '"')
                break;

            *quote_end = '\0';
            char *file = fl_io_realpath_realloc(sbs_io_to_host_path_realloc(build->context->host->os, fl_cstring_vdup("%s%s%s", path, (strlen(path) > 0 ? (char[]){ build->context->host->dir_separator, '\0' } : ""), include)));

            if (fl_io_file_exists(file))
            {
                bool need_to_visit = true;
                for (size_t i=0; need_to_visit && i < fl_vector_length(visited_files); i++)
                {
                    if (flm_cstring_equals(file, *(char**) fl_vector_ref_get(visited_files, i)))
                        need_to_visit = false;
                }

                if (need_to_visit)
                {
                    fl_vector_add(visited_files, &file);
                    resolve_c_file_dependencies(build, file, resolved_files, include_folders, visited_files);
                    *resolved_files = fl_array_append(*resolved_files, &file);
                }
                else
                {
                    fl_cstring_free(file);
                }
            }
            else
            {
                fl_cstring_free(file);
            }

            include = ++quote_end;
        }
        else if (include_folders != NULL && include != end && *include == '<')
        {
            // Skip the starting angle
            include++;

            char *angle_end = include;
            
            while (true)
            {
                if (*angle_end == '\n' || *angle_end == '>')
                    break;
                
                angle_end++;
            }

            if (!angle_end || *angle_end != '>')
                break;

            *angle_end = '\0';

            for (size_t i=0; i < fl_array_length(include_folders); i++)
            {
                char *file = fl_io_realpath_realloc(sbs_io_to_host_path_realloc(build->context->host->os, fl_cstring_vdup("%s%s%s", include_folders[i], (char[]){ build->context->host->dir_separator, '\0' }, include)));

                if (fl_io_file_exists(file))
                {
                    bool need_to_visit = true;
                    for (size_t i=0; need_to_visit && i < fl_vector_length(visited_files); i++)
                    {
                        if (flm_cstring_equals(file, *(char**) fl_vector_ref_get(visited_files, i)))
                            need_to_visit = false;
                    }

                    if (need_to_visit)
                    {
                        fl_vector_add(visited_files, &file);
                        resolve_c_file_dependencies(build, file, resolved_files, include_folders, visited_files);
                        *resolved_files = fl_array_append(*resolved_files, &file);
                    }
                    else
                    {
                        fl_cstring_free(file);
                    }
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

static char** resolve_source_files(SbsBuild *build, const char **target_sources)
{
    char** source_files = fl_array_new(sizeof(const char*), 0);

    for (size_t i=0; i < fl_array_length(target_sources); i++)
    {
        char *target_source = sbs_io_to_host_path(build->context->host->os, target_sources[i]);

        if (fl_io_file_exists(target_source))
        {
            const char *source_file = sbs_io_to_host_path(build->context->env->host->os, target_source);
            source_files = fl_array_append(source_files, &source_file);
        }
        else
        {
            // Escape the backslash because target_source is used as a regex pattern:
            //  On Linux: "/an/example\ with/space" -> "/an/example\\ with/space"
            //  On Windows: "C:\\an\\example\\" -> "C:\\\\an\\\\example\\\\"
            target_source = fl_cstring_replace_realloc(target_source, "\\", "\\\\");

            char **matched_files = fl_io_file_find(target_source, build->context->host->dir_separator);

            if (matched_files)
            {
                for (size_t i=0; i < fl_array_length(matched_files); i++)
                {
                    char *matched_file = sbs_io_to_host_path_realloc(build->context->env->host->os, matched_files[i]);
                    source_files = fl_array_append(source_files, &matched_file);
                }
                fl_array_free(matched_files);
            }
        }

        fl_cstring_free(target_source);
    }

    return source_files;
}

static char* build_object_filename(const SbsBuild *build, const SbsConfigCompile *config_compile, const char *source_file, const char *output_dir)
{
    // We need to build the object filename using the source filename for it
    //  ex: main.c -> main.o
    char *object_file = NULL;

    FlVector *source_file_parts = fl_cstring_split_by(source_file, (char[]){ build->context->env->host->dir_separator, '\0' });

    // Remove "current directory" if present
    if (flm_vector_first(source_file_parts, char*)[0] == '.')
        fl_vector_shift(source_file_parts, NULL);

    // Get the filename
    char *filename = NULL;
    fl_vector_pop(source_file_parts, &filename);

    // If the user supplied an extension, use it. If not take the default ".o"
    const char *extension = config_compile->extension ? config_compile->extension : ".o";

    filename = fl_cstring_replace_realloc(filename, ".cpp", extension);
    filename = fl_cstring_replace_realloc(filename, ".c", extension);

    // Get the source file path to replicate it under the output path
    //  ex: from the original filename "src/path/to/file.c" we get "src/path/to/"
    char *source_file_path = fl_cstring_join(source_file_parts, (char[]) { build->context->env->host->dir_separator, '\0' });

    // Start building the output path
    char *output_file_fullpath = sbs_io_to_host_path(build->context->env->host->os, output_dir);

    if (output_file_fullpath[strlen(output_file_fullpath) - 1] != build->context->env->host->dir_separator)
        fl_cstring_append_char(&output_file_fullpath, build->context->env->host->dir_separator);

    output_file_fullpath = sbs_context_interpolate_string_realloc(build->context, output_file_fullpath);

    // Append the source path structure
    fl_cstring_append(&output_file_fullpath, source_file_path);

    // Create the output directory
    fl_io_dir_create_recursive(output_file_fullpath);

    // Create the full path for the object file
    object_file = fl_cstring_vdup("%s%c%s", output_file_fullpath, build->context->env->host->dir_separator, filename);

    fl_cstring_free(output_file_fullpath);
    fl_cstring_free(source_file_path);
    fl_cstring_free(filename);
    fl_vector_free(source_file_parts);

    return object_file;
}

char** sbs_build_compile(SbsBuild *build)
{
    SbsTargetCompile *target_compile = (SbsTargetCompile*) build->current_target;

    if (!target_compile || !target_compile->sources)
        return NULL;

    const SbsConfigCompile *config_compile = &build->context->config->compile;

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

    // Create the includes string gluing the include directories with the include_dir_flag
    char *includes = fl_cstring_new(0);
    for (size_t i = 0; target_compile->includes && i < fl_array_length(target_compile->includes); i++)
    {
        fl_cstring_append(&includes, build->context->toolchain->compiler.include_dir_flag);
        sbs_string_append_free(&includes, sbs_io_to_host_path(build->context->env->host->os, target_compile->includes[i]));
        fl_cstring_append(&includes, " ");
    }

    // Create the defines string gluing the flags with the compiler define_flag
    char *defines = fl_cstring_new(0);
    if (target_compile->defines)
    {
        for (size_t i = 0; i < fl_array_length(target_compile->defines); i++)
        {
            fl_cstring_append(&defines, build->context->toolchain->compiler.define_flag);
            fl_cstring_append(&defines, target_compile->defines[i]);
            fl_cstring_append(&defines, " ");
        }
    }

    // Get all the source files of this compile target
    char **target_source_files = resolve_source_files(build, (const char**) target_compile->sources);

    size_t n_sources = fl_array_length(target_source_files);

    // Keep track of the objects
    char **objects = fl_array_new(sizeof(char*), n_sources);

    // Compile source objects
    bool success = true;
    for (size_t i = 0; success && i < n_sources; i++)
    {
        // Unify path separators for the source file
        char *source_file = sbs_io_to_host_path(build->context->env->host->os, target_source_files[i]);

        // Get the source's last modification timestamp
        unsigned long long source_timestamp;
        fl_io_file_get_modified_timestamp(source_file, &source_timestamp);

        // Get the object file path
        char *object_file = build_object_filename(build, config_compile, source_file, target_compile->output_dir);

        /// Get the target command's file
        char *object_tc_file = fl_cstring_vdup("%s.stc", object_file);

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
                char **deps = fl_array_new(sizeof(char*), 0);
                resolve_c_file_dependencies(build, source_file, &deps, target_compile->includes, NULL);

                for (size_t i=0; i < fl_array_length(deps); i++)
                {
                    char *dep_file = deps[i];
            
                    if (fl_io_file_exists(dep_file))
                    {
                        unsigned long long dep_file_timestamp;
                        if (fl_io_file_get_modified_timestamp(dep_file, &dep_file_timestamp) && object_timestamp < dep_file_timestamp)
                        {
                            needs_compile = true;
                            break;
                        }
                    }
                    else
                    {
                        // If the file does not exist, we trigger the (re)compilation to make sure
                        // we are using the right dependencies
                        needs_compile = true;
                    }
                }

                fl_array_free_each_pointer(deps, (FlArrayFreeElementFunc) fl_cstring_free);
            }
        }

        if (build->context->toolchain->compiler.bin)
        {
            // Replace the special ${sbs.input_file} and ${sbs.output_file} variables in the falgs
            char *compilation_unit_flags = fl_cstring_replace(flags, "${sbs.input_file}", source_file);
            compilation_unit_flags = fl_cstring_replace_realloc(compilation_unit_flags, "${sbs.output_file}", object_file);

            // Build the compile command
            char *command = fl_cstring_vdup("%s %s %s %s", build->context->toolchain->compiler.bin, defines, includes, compilation_unit_flags);

            if (fl_io_file_exists(object_tc_file))
            {
                char *previous_command = fl_io_file_read_all_text(object_tc_file);

                if (previous_command != NULL && !flm_cstring_equals(previous_command, command))
                    needs_compile = true;

                fl_cstring_free(previous_command);
            }

            if (needs_compile || build->script_mode)
            {
                // Exec
                success = sbs_executor_run_command(build->context->executor, command) && success;
                
                // Update the last flags used for this translation unit
                if (success && !build->script_mode)
                    fl_io_file_write_all_text(object_tc_file, command);
            }
            else
            {
                fprintf(stdout, "File '%s' has not changed. Skipping compilation...\n", source_file);
            }
            
            // clean
            fl_cstring_free(command);
            fl_cstring_free(compilation_unit_flags);
        }
        else
        {
            success = false;
            fprintf(stdout, "Toolchain '%s' does not have a compiler executable defined for environment '%s'", build->context->toolchain->name, build->context->env->name);
        }

        fl_cstring_free(object_tc_file);
        fl_cstring_free(source_file);
    }

    fl_array_free_each_pointer(target_source_files, (FlArrayFreeElementFunc) fl_cstring_free);
    fl_cstring_free(defines);
    fl_cstring_free(includes);
    fl_cstring_free(flags);

    if (!success)
    {
        fl_array_free_each_pointer(objects, (FlArrayFreeElementFunc) fl_cstring_free);
        return NULL;
    }

    return objects;
}
