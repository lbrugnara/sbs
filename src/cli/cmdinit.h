#ifndef SBS_CMDINIT_H
#define SBS_CMDINIT_H

#include <stddef.h>
#include "../result.h"

/*
 * Function: sbs_command_init
 *  Creates an empty build file if it doesn't exist
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *  argv_offset - The offset is used to skip the main sbs command args
 *
 * Returns:
 * SbsResult - SBS_RES_OK if the files does not exist and it is created. Otherwise an error code.
 */
SbsResult sbs_command_init(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDINIT_H */
