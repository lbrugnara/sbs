#ifndef SBS_CMDHELP_H
#define SBS_CMDHELP_H

#include "../result.h"

/*
 * Function: sbs_command_help
 *  Shows the help message 
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  SbsResult - It always returns SBS_RES_OK
 */
SbsResult sbs_command_help(int argc, char **argv, char **env);

#endif /* SBS_CMDHELP_H */
