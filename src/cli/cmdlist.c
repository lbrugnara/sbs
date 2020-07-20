#include <stdio.h>
#include "args.h"
#include "cli.h"
#include "cmdlist.h"
#include "../common/result.h"
#include "../build/build.h"
#include "../parser/file.h"

struct SbsListArguments {
    char *file;
    char *resource;
};

static const char *list_help =
    "Usage: sbs list <options>? <resource>                                          \n"
    "                                                                               \n"
    "    options:                                                                   \n"
    "       --help, -h                  Prints this message                         \n"
    "       --file=<path>, -f=<path>    Build file's path. Default value is         \n"
    "                                   .sbs/build.sbs                              \n"
    "    resource:                                                                  \n"
    "       One of the following values that identify the different set of          \n"
    "       user-defined objects that can be declared within a build file:          \n"
    "                                                                               \n"
    "           envs            Shows the list of available environments            \n"
    "           toolchains      Shows the list of available toolchains              \n"
    "           configs         Shows the list of the available configurations      \n"
    "           targets         Shows all the targets available to be built         \n"
    "           presets         Shows the list of available presets                 "
;

static const char *list_args_error =
    "\n"
    "Use 'sbs list [-h|--help]' to know how to invoke the list command              \n"
;

static int compare_strings(const void* a, const void* b) 
{ 
    return strcmp(*(const char**)a, *(const char**)b); 
}

SbsResult sbs_command_list(int argc, char **argv, char **env)
{
    sbs_cli_print_header();

    // If the are no arguments, print the help message
    if (argc == 2)
    {
        sbs_cli_print_message(list_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
        return SBS_RES_WRONG_ARGS;
    }

    struct SbsListArguments args = { 0 };
    
    // A boolean to pass to the sbs_parse_args macro to save the parsing result
    SbsArgsResult parsed_args = SBS_ARGS_OK;
    
    // args+2: skip program name and "build" argument
    sbs_parse_args(argv+2, {
        use_retval(&parsed_args);
        use_error_func(sbs_cli_print_error);
        with_help("--help", "-h");
        with_options(
            command_any(args.resource)
            flag_string("--file", "-f", &args.file)
        );
    });

    if (parsed_args == SBS_ARGS_HELP)
    {
        sbs_cli_print_message(list_help);
        return SBS_RES_OK;
    }

    if (parsed_args == SBS_ARGS_ERROR)
    {
        sbs_cli_print_error(list_args_error);
        return SBS_RES_WRONG_ARGS;
    }

    // If the resource is not present, show the help message and return an error
    if (args.resource == NULL)
    {
        sbs_cli_print_message(list_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
        return SBS_RES_MISSING_RESOURCE_ARG;
    }

    // If present the file argument, make sure the filename is valid
    if (args.file && strlen(args.file) == 0)
    {
        sbs_cli_print_header();
        sbs_cli_print_error("File name cannot be empty");
        return SBS_RES_INVALID_FILE;
    }

    // If present the file argument, make sure the file exists
    if (args.file && !fl_io_file_exists(args.file))
    {
        sbs_cli_print_header();
        const char *error = sbs_result_get_reason(SBS_RES_INVALID_FILE, args.file);
        sbs_cli_print_error("%s", error);
        fl_cstring_free(error);
        return SBS_RES_INVALID_FILE;
    }
    
    // Parse the build file to get all the resources
    SbsFile *file = sbs_file_parse(args.file ? args.file : ".sbs/build.sbs");

    // Get the requested resource. If the resource name is not valid, show an error.
    const char *resource_name = NULL;
    FlHashtable *target_resource  = NULL;

    if (flm_cstring_equals("envs", args.resource))
    {
        target_resource = file->envs;
        resource_name = "environments";
    }
    else if (flm_cstring_equals("toolchains", args.resource))
    {
        target_resource = file->toolchains;
        resource_name = "toolchains";
    }
    else if (flm_cstring_equals("configs", args.resource))
    {
        target_resource = file->configurations;
        resource_name = "configurations";
    }
    else if (flm_cstring_equals("actions", args.resource))
    {
        target_resource = file->actions;
        resource_name = "actions";
    }
    else if (flm_cstring_equals("targets", args.resource))
    {
        target_resource = file->targets;
        resource_name = "targets";
    }
    else if (flm_cstring_equals("presets", args.resource))
    {
        target_resource = file->presets;
        resource_name = "presets";
    }
    else
    {
        sbs_file_free(file);
        sbs_cli_print_header();
        sbs_cli_print_error("Unkown resource type '%s'", args.resource);
        return SBS_RES_INVALID_RESOURCE;
    }

    // Get all the keys that are also the names
    char **keys = fl_hashtable_keys(target_resource);

    // If there are no keys it means there are no items of the requested resource type
    if (!keys)
    {
        sbs_cli_print_message("There are no %s in build file %s", resource_name, file->filename);
        sbs_file_free(file);
        return SBS_RES_OK;
    }

    // Print the ordered list of requested resources
    qsort(keys, fl_array_length(keys), sizeof(char*), compare_strings); 

    sbs_cli_print_header();

    sbs_cli_print_message("List of %s in the build file %s:\n", resource_name, file->filename);
    for (size_t i=0; i < fl_array_length(keys); i++)
        sbs_cli_print_message("    %s", keys[i]);
    
    fl_array_free(keys);
    sbs_file_free(file);

    return SBS_RES_OK;
}
