#ifndef SBS_CMDBUILD_H
#define SBS_CMDBUILD_H

#include "../common/result.h"

/*
 * Function: sbs_command_build
 *  Runs a build process using the provided arguments
 *
 * Parameters:
 *  argc - Arguments count
 *  argv - Arguments array
 *  env - Environment array
 *
 * Returns:
 *  SbsResult - SBS_RES_OK if the build process is successful. Otherwise an error code
 */
SbsResult sbs_command_build(int argc, char **argv, char **env);

#endif /* SBS_CMDBUILD_H */
