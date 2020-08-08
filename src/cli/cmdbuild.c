#include <stdio.h>
#include "cmdbuild.h"
#include "args.h"
#include "cli.h"
#include "../io.h"
#include "../result.h"
#include "../build/build.h"
#include "../lang/file.h"

static const char *build_help =
    "Usage: sbs build [ <preset> <options>? <resources>? | <options>? <resources> ] \n"
    "                                                                               \n"
    "    preset:                                                                    \n"
    "       An identifier for a preset that contains a predefined set of resources  \n"
    "       to use in the build process. Each preset's resource can be overridden   \n"
    "       using the resources listed below.                                       \n"
    "       A preset can omit some resources, in that case the specific flag for the\n"
    "       missing resource is required (see below).                               \n"
    "       To get a list of the available presets run \"sbs list presets\" .       \n"
    "                                                                               \n"
    "    options:                                                                   \n"
    "       --help, -h                  Prints this message                         \n"
    "       --file=<path>, -f=<path>    Build file's path. Default value is         \n"
    "                                   .sbs/build.sbs                              \n"
    "                                                                               \n"
    "    resources:                                                                 \n"
    "       Resources to be used in the build process. If the preset name is not    \n"
    "       provided, all these resources are required by the build process.        \n"
    "                                                                               \n"
    "       --env=<name>, -e=<name>             Environment to be used in the build \n"
    "                                           process                             \n"
    "                                                                               \n"
    "       --toolchain=<name>, -tc=<name>      Toolchain to build the target       \n"
    "                                                                               \n"
    "       --config=<name>, -c=<name>          Configuration used to build the     \n"
    "                                           target                              \n"
    "                                                                               \n"
    "       --target=<name>, -t=<name>          Target's name to be built           \n"
    "                                                                               \n"
    "    For more information on the available resources, run:                      \n"
    "        sbs list --help                                                        "
;

static const char *build_args_error =
    "\n"
    "Use 'sbs build [-h|--help]' to know how to invoke the build command            \n"
;

SbsResult sbs_command_build(int argc, char **argv, char **env, size_t argv_offset)
{
    sbs_cli_print_header();

    // If the are no arguments, print the help message
    if (argc == 2)
    {
        sbs_cli_print_message(build_help);
        return SBS_RES_WRONG_ARGS;
    }

    SbsBuildArgs args = { 0 };
    args.argc = argc;
    args.argv = argv;
    args.env = env;

    char *build_file_path = NULL;

    SbsArgsResult parsed_args = SBS_ARGS_OK;
    // args+2: skip program name and "build" argument
    sbs_args_parse(argv + argv_offset, {
        sbs_args_retval(&parsed_args);
        sbs_args_error_fn(sbs_cli_print_error);
        sbs_args_help("--help", "-h");
        sbs_args_list(
            sbs_args_word(args.preset)
            sbs_args_string("--env", "-e", &args.environment)
            sbs_args_string("--toolchain", "-tc", &args.toolchain)
            sbs_args_string("--config", "-c", &args.config)
            sbs_args_string("--target", "-t", &args.target)
            sbs_args_string("--file", "-f", &build_file_path)
            sbs_args_string("--working-dir", "-cwd", &args.cwd)
            sbs_args_flag("--script-mode", "-s", &args.script_mode)
            sbs_args_string("--arch", "-a", &args.arch)
        );
    });

    if (parsed_args == SBS_ARGS_HELP)
    {
        sbs_cli_print_message(build_help);
        return SBS_RES_OK;
    }

    if (parsed_args == SBS_ARGS_ERROR)
    {
        sbs_cli_print_error(build_args_error);
        return SBS_RES_WRONG_ARGS;
    }

    // Check if the processed arguments are valid
    if (args.preset == NULL  && (args.environment == NULL || args.toolchain == NULL || args.config == NULL || args.target == NULL))
    {
        sbs_cli_print_message(build_help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
        return SBS_RES_WRONG_ARGS;
    }

    // If present the file argument, make sure the filename is valid
    if (build_file_path != NULL && strlen(build_file_path) == 0)
    {
        sbs_cli_print_error("File name cannot be empty");
        return SBS_RES_INVALID_FILE;
    }

    if (args.cwd)
    {
        if (!fl_system_set_working_dir(args.cwd))
        {
            sbs_cli_print_error("Could not change working directory to %s", args.cwd);
            return SBS_RES_INVALID_FILE;
        }
    }

    // Convert the file path to an absolute path
    char *file_abspath = sbs_io_realpath((build_file_path ? build_file_path : ".sbs/build.sbs"));

    if (file_abspath == NULL || !fl_io_file_exists(file_abspath))
    {
        const char *error = sbs_result_get_reason(SBS_RES_INVALID_FILE, file_abspath);
        sbs_cli_print_error("%s", error);
        fl_cstring_free(error);
        return SBS_RES_INVALID_FILE;
    }
    
    // Parse the build file to get all the resources
    SbsFile *file = sbs_file_parse(file_abspath);
    fl_cstring_free(file_abspath);

    // If the preset argument is present, make sure it exists
    if (args.preset != NULL && !fl_hashtable_has_key(file->presets, args.preset))
    {
        sbs_file_free(file);
        return SBS_RES_INVALID_PRESET;
    }
    
    // Run the build process and leave
    SbsResult result = sbs_build_run(file, &args);

    sbs_file_free(file);

    return result;
}
