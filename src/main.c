#include <fllib/os/System.h>
#include "cli/cli.h"
#include "cli/args.h"
#include "cli/cmdrun.h"
#include "cli/cmdbuild.h"
#include "cli/cmdhelp.h"
#include "cli/cmdinit.h"
#include "cli/cmdlist.h"

#ifndef SBS_TESTS
int main(int argc, char **argv, char **env)
{
    // Default argv offset is used by the subcommands to skip
    // the program name and the parsed arguments that belong to the
    // main command
    size_t argv_offset = 1;

    if (argc == 1)
    {
        // No arguments? Show the help message
        sbs_command_help(argc, argv, env, argv_offset);
        return SBS_RES_ERROR;
    }

    SbsArgsResult parsed_args = SBS_ARGS_OK;
    char *subcommand = NULL;
    char *flag_cwd = NULL;
    {
        sbs_args_parse(argv+1, {
            sbs_args_retval(&parsed_args);
            sbs_args_error_fn(sbs_cli_print_error);
            sbs_args_help("--help", "-h");
            sbs_args_list(
                sbs_args_subcmd(subcommand, "init", "list", "build")
                sbs_args_string("--working-dir", "-cwd", &flag_cwd)
            );
        });
    }    

    // Increment the offset for each parsed argument
    if (subcommand != NULL) argv_offset++;
    if (flag_cwd != NULL) argv_offset++;

    // Show the help message if requested
    if (parsed_args == SBS_ARGS_HELP)
        return sbs_command_help(argc, argv, env, argv_offset);

    if (flag_cwd)
    {
        if (!fl_system_set_working_dir(flag_cwd))
        {
            sbs_cli_print_error("Could not change working directory to %s", flag_cwd);
            return SBS_RES_INVALID_FILE;
        }
    }

    // Check for subcommands
    if (sbs_args_is_cmd("init", subcommand))
        return sbs_command_init(argc, argv, env, argv_offset);
    
    if (sbs_args_is_cmd("list", subcommand))
        return sbs_command_list(argc, argv, env, argv_offset);
    
    if (sbs_args_is_cmd("build", subcommand))
        return sbs_command_build(argc, argv, env, argv_offset);

    // Default command is running an action
    return sbs_command_run(argc, argv, env, argv_offset);
}
#endif
