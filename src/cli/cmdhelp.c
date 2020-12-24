#include <stdio.h>
#include "cli.h"
#include "../version.h"
#include "../result.h"

static const char *help = 
    CLI_HEADER
    "Usage: sbs [ <options> | <command> ]                                           \n"
    "                                                                               \n"
    "    options:                                                                   \n"
    "       --help, -h      Prints this message                                     \n"
    "                                                                               \n"
    "    command:                                                                   \n"
    "       init            Initializes the sbs system in the current directory     \n"
    "       list            Prints the list of user defined elements for a resource \n"
    "       build           Starts the build process                                \n"
    "                                                                               \n"
    "    To get more information about the commands you can run:                    \n"
    "       sbs <command> --help                                                    "
;

SbsResult sbs_command_help(int argc, char **argv, char **env, size_t argv_offset)
{
    sbs_cli_print_message(help, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
    return SBS_RES_OK;
}
