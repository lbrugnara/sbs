#ifndef SBS_CMDBUILD_H
#define SBS_CMDBUILD_H

#include <stddef.h>
#include "../result.h"

/*
 * Function: sbs_command_build
 *  Runs a build process using the provided arguments
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *  argv_offset - The offset is used to skip the main sbs command args
 *
 * Returns:
 *  SbsResult - SBS_RES_OK if the build process is successful. Otherwise an error code
 */
SbsResult sbs_command_build(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDBUILD_H */
