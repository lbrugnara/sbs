#include "archive.h"
#include "build.h"

#define SBS_DIR_SEPARATOR "/"

FlVector sbs_build_target_archive(struct SbsBuild *build)
{
    struct SbsTargetArchive *archive = (struct SbsTargetArchive*)build->target;

    // Collect all the archive flags in the configuration hierarchy
    char *flags = fl_cstring_dup("");
    if (build->config->archive.flags)
    {
        for (size_t i = 0; i < fl_array_length(build->config->archive.flags); i++)
        {
            fl_cstring_append(&flags, build->config->archive.flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    // File
    const char *extension = build->config->archive.extension ? build->config->archive.extension : ".a";

    // Path
    char *output_file_fullpath = fl_cstring_dup(archive->base.output_dir);
    fl_cstring_append(&output_file_fullpath, SBS_DIR_SEPARATOR);
    fl_cstring_append(&output_file_fullpath, build->config->name);

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

            FlVector target_objects = sbs_build_target(&(struct SbsBuild) {
                .executor = build->executor,
                .file = build->file,
                .env = build->env,
                .toolchain = build->toolchain,
                .target = fl_hashtable_get(build->file->targets, archive->objects[i].value),
                .config = build->config
            });

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
            char *command = fl_cstring_vdup("%s %s", build->toolchain->archiver, archive_flags);

            for (size_t i=0; i < fl_vector_length(objects); i++)
                fl_cstring_append(fl_cstring_append(&command, " "), fl_vector_get(objects, i));

            // Exec
            success = sbs_executor_run_command(build->executor, command) && success;

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
