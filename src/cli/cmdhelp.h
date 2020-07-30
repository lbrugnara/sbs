#ifndef SBS_CMDHELP_H
#define SBS_CMDHELP_H

#include <stddef.h>
#include "../result.h"

/*
 * Function: sbs_command_help
 *  Shows the help message 
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *  argv_offset - The offset is used to skip the main sbs command args
 *
 * Returns:
 *  SbsResult - It always returns SBS_RES_OK
 */
SbsResult sbs_command_help(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDHELP_H */
