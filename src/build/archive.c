#include "archive.h"
#include "build.h"

#define SBS_DIR_SEPARATOR "/"

static char* build_output_filename(struct SbsBuild *build, struct SbsConfigArchive *archive, const char *output_dir, const char *output_name)
{
    // File
    const char *extension = archive->extension ? archive->extension : ".a";

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

FlVector sbs_build_target_archive(struct SbsBuild *build)
{
    struct SbsTargetArchive *target_archive = (struct SbsTargetArchive*)build->target;
    struct SbsConfigArchive *config_archive = sbs_config_archive_get(build->config, build->env->name);

    // Collect all the archive flags in the configuration hierarchy
    char *flags = fl_cstring_new(0);
    if (config_archive->flags)
    {
        for (size_t i = 0; i < fl_array_length(config_archive->flags); i++)
        {
            fl_cstring_append(&flags, config_archive->flags[i]);
            fl_cstring_append(&flags, " ");
        }
    }

    // Build the target's output filename
    char *output_filename = build_output_filename(build, config_archive, target_archive->base.output_dir, target_archive->output_name);

    // We add the filename to the output vector that will return the target's result
    FlVector output = fl_vector_new(1, fl_container_cleaner_pointer);
    fl_vector_add(output, output_filename);

    // Get the timestamp of the output file (if it exists)
    unsigned long long archive_timestamp = 0;
    fl_io_file_get_modified_timestamp(output_filename, &archive_timestamp);

    // By default we assume we don't need to run the command (see below)
    bool needs_archive = false;    

    // This vector will keep track of the objects that are needed to build the archive
    FlVector archive_objects = fl_vector_new(10, fl_container_cleaner_pointer);    

    // We iterate through all the archive's objects where we can find two type of resources:
    //  1- An identifier that represents another target, we need to build that target and use its output
    //  2- An string that is the path to a file that can be directly included in the archive
    bool success = true;
    for (size_t i = 0; i < fl_array_length(target_archive->objects); i++)
    {
        if (target_archive->objects[i].type == SBS_IDENTIFIER)
        {
            // Build the target
            FlVector target_objects = sbs_build_target(&(struct SbsBuild) {
                .executor = build->executor,
                .file = build->file,
                .env = build->env,
                .toolchain = build->toolchain,
                .target = fl_hashtable_get(build->file->targets, target_archive->objects[i].value),
                .config = build->config
            });

            // Something odd happened if it is null, we need to leave with error
            if (target_objects == NULL)
            {
                success = false;
                break;
            }

            // Check every object file to see if it is newer than the last archive build's
            // modification timestamp. If there is just one object newer to the archive, we need to
            // run the build.
            while (fl_vector_length(target_objects) > 0)
            {
                char *obj = NULL;
                fl_vector_shift(target_objects, &obj);

                unsigned long long obj_timestamp = 0;
                if (!fl_io_file_get_modified_timestamp(obj, &obj_timestamp))
                    needs_archive = true;

                if (archive_timestamp < obj_timestamp)
                    needs_archive = true;

                fl_vector_add(archive_objects, obj);
            }
            fl_vector_delete(target_objects);
        }
        else
        {
            // We also check here to see if the object pointed by the string is newer than the archive
            // in order to set the needs_archive fkag
            unsigned long long obj_timestamp;
            if (!fl_io_file_get_modified_timestamp(target_archive->objects[i].value, &obj_timestamp))
                needs_archive = true;

            if (archive_timestamp < obj_timestamp)
                needs_archive = true;

            fl_vector_add(archive_objects, target_archive->objects[i].value);
        }

        if (needs_archive)
        {
            const char *archiver = sbs_toolchain_get_archiver(build->toolchain, build->env);

            if (archiver != NULL)
            {
                // Replace the special ${archive} variable in the flag
                char *archive_flags = fl_cstring_replace(flags, "${archive}", output_filename);

                // Build the compile command
                char *command = fl_cstring_vdup("%s %s", archiver, archive_flags);

                for (size_t i=0; i < fl_vector_length(archive_objects); i++)
                    fl_cstring_append(fl_cstring_append(&command, " "), fl_vector_get(archive_objects, i));

                // Exec
                success = sbs_executor_run_command(build->executor, command) && success;

                fl_cstring_delete(command);
                fl_cstring_delete(archive_flags);
            }
            else
            {
                success = false;
                fprintf(stdout, "Toolchain '%s' does not have an archiver executable defined for environment '%s'", build->toolchain->name, build->env->name);
            }
        }
        else
        {
            fprintf(stdout, "File '%s' has not changed. Skipping target...", output_filename);
        }
    }

    fl_vector_delete(archive_objects);
    fl_cstring_delete(flags);
    sbs_config_archive_free(config_archive);

    if (!success)
    {
        fl_vector_delete(output);
        return NULL;
    }

    return output;
}
