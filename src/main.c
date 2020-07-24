#include "cli/args.h"
#include "cli/cmdrun.h"
#include "cli/cmdbuild.h"
#include "cli/cmdhelp.h"
#include "cli/cmdinit.h"
#include "cli/cmdlist.h"

#ifndef SBS_TESTS
int main(int argc, char **argv, char **env)
{
    if (argc == 1)
    {
        // No arguments? Show the help message
        sbs_command_help(argc, argv, env);
        return SBS_RES_ERROR;
    }

    // Show the help message if requested
    if (sbs_args_is_flag("--help", "-h", argv[1]))
        return sbs_command_help(argc, argv, env);

    // Check for subcommands
    if (sbs_args_is_cmd("init", argv[1]))
        return sbs_command_init(argc, argv, env);
    
    if (sbs_args_is_cmd("list", argv[1]))
        return sbs_command_list(argc, argv, env);
    
    if (sbs_args_is_cmd("build", argv[1]))
        return sbs_command_build(argc, argv, env);    

    // Default command is running an action
    return sbs_command_run(argc, argv, env);
}
#endif
