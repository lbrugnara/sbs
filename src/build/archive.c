#include "archive.h"
#include "build.h"
#include "../io.h"
#include "../utils.h"
#include "../objects/configuration.h"
#include "../objects/toolchain.h"

static char* build_output_filename(SbsBuild *build, const SbsConfigArchive *archive, const char *output_dir, const char *output_name)
{
    // File
    const char *extension = archive->extension ? archive->extension : ".a";

    // Path
    char *output_filename = sbs_io_to_host_path(build->context->env->host->os, output_dir);

    if (output_filename[strlen(output_filename) - 1] != build->context->env->host->dir_separator)
        fl_cstring_append_char(&output_filename, build->context->env->host->dir_separator);

    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.os}", sbs_host_os_to_str(build->context->host->os));
    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.arch}", sbs_host_arch_to_str(build->context->host->arch));
    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.env}", build->context->env->name);
    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.config}", build->context->config->name);
    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.target}", build->context->target->name);
    output_filename = fl_cstring_replace_realloc(output_filename, "${sbs.toolchain}", build->context->toolchain->name);

    fl_io_dir_create_recursive(output_filename);

    // Create the fullname
    fl_cstring_append(fl_cstring_append(&output_filename, output_name), extension);

    return output_filename;
}

char** sbs_build_target_archive(SbsBuild *build)
{
    SbsTargetArchive *target_archive = (SbsTargetArchive*) build->context->target;
    const SbsConfigArchive *config_archive = &build->context->config->archive;

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
    char *output_filename = build_output_filename(build, config_archive, target_archive->output_dir, target_archive->output_name);

    // We add the filename to the output vector that will return the target's result
    char **output = fl_array_new(sizeof(char*), 1);
    output[0] = output_filename;

    // Get the timestamp of the output file (if it exists)
    unsigned long long archive_timestamp = 0;
    fl_io_file_get_modified_timestamp(output_filename, &archive_timestamp);

    // By default we assume we don't need to run the command (see below)
    bool needs_archive = false;    

    // This vector will keep track of the objects that are needed to build the archive
    size_t n_objects = fl_array_length(target_archive->objects);
    FlVector *archive_objects = flm_vector_new_with(.capacity = n_objects, .cleaner = fl_container_cleaner_pointer);    

    // We iterate through all the archive's objects where we can find two type of resources:
    //  1- An identifier that represents another target, we need to build that target and use its output
    //  2- An string that is the path to a file that can be directly included in the archive
    bool success = true;
    for (size_t i = 0; i < n_objects; i++)
    {
        if (target_archive->objects[i].type == SBS_IDENTIFIER)
        {
            SbsContext *tmpctx = sbs_context_copy(build->context);
            sbs_target_free(tmpctx->target);
            tmpctx->target = sbs_target_resolve(build->context, target_archive->objects[i].value, (const SbsTarget*) target_archive);

            // target_objects is an array of pointers to char allocated by the target
            char **target_objects = sbs_build_target(&(SbsBuild) {
                .context = tmpctx,
                .script_mode = build->script_mode
            });

            sbs_context_free(tmpctx);

            // Something odd happened if it is null, we need to leave with error
            if (target_objects == NULL)
            {
                success = false;
                break;
            }

            // Check every object file to see if it is newer than the last archive build's
            // modification timestamp. If there is just one object newer to the archive, we need to
            // run the build.
            for (size_t i=0; i < fl_array_length(target_objects); i++)
            {
                char *obj = target_objects[i];
                target_objects[i] = NULL;

                unsigned long long obj_timestamp = 0;
                if (!fl_io_file_get_modified_timestamp(obj, &obj_timestamp))
                    needs_archive = true;

                if (archive_timestamp < obj_timestamp)
                    needs_archive = true;

                fl_vector_add(archive_objects, &obj);
            }

            fl_array_free(target_objects);
        }
        else
        {
            // We also check here to see if the object pointed by the string is newer than the archive
            // in order to set the needs_archive flag
            unsigned long long obj_timestamp;
            if (!fl_io_file_get_modified_timestamp(target_archive->objects[i].value, &obj_timestamp))
                needs_archive = true;

            if (archive_timestamp < obj_timestamp)
                needs_archive = true;

            char *object_file = sbs_io_to_host_path(build->context->env->host->os, target_archive->objects[i].value);
            fl_vector_add(archive_objects, &object_file);
        }        
    }

    if (success && (needs_archive || build->script_mode))
    {
        if (build->context->toolchain->archiver.bin != NULL)
        {
            // Replace the special ${sbs.output_file} variable in the flag
            char *archive_flags = fl_cstring_replace(flags, "${sbs.output_file}", output_filename);

            // Build the compile command
            char *command = fl_cstring_vdup("%s %s", build->context->toolchain->archiver.bin, archive_flags);

            for (size_t i=0; i < fl_vector_length(archive_objects); i++)
                fl_cstring_append(fl_cstring_append(&command, " "), *(char**) fl_vector_ref_get(archive_objects, i));

            // Exec
            success = sbs_executor_run_command(build->context->executor, command) && success;

            fl_cstring_free(command);
            fl_cstring_free(archive_flags);
        }
        else
        {
            success = false;
            fprintf(stdout, "Toolchain '%s' does not have an archiver executable defined for environment '%s'", build->context->toolchain->name, build->context->env->name);
        }
    }
    else if (success)
    {
        fprintf(stdout, "File '%s' has not changed. Skipping target...\n", output_filename);
    }
    else
    {
        fprintf(stdout, "A dependency of '%s' is missing or not valid, could not build output file.\n", output_filename);
    }

    fl_vector_free(archive_objects);
    fl_cstring_free(flags);

    if (!success)
    {
        fl_array_free_each_pointer(output, (FlArrayFreeElementFunc) fl_cstring_free);
        return NULL;
    }

    return output;
}
