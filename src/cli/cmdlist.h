#ifndef SBS_CMDLIST_H
#define SBS_CMDLIST_H

#include <stddef.h>
#include "../result.h"

/*
 * Function: sbs_command_list
 *  Prints a list of all the requested resources available in the build file
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *  argv_offset - The offset is used to skip the main sbs command args
 *
 * Returns:
 *  SbsResult - SBS_RES_OK if the resources are listed. Otherwise an error code.
 */
SbsResult sbs_command_list(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDLIST_H */
