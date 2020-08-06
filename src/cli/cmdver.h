#ifndef SBS_CMDVER_H
#define SBS_CMDVER_H

#include <stddef.h>
#include "../result.h"

/*
 * Function: sbs_command_version
 *  Shows the information about the program
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
SbsResult sbs_command_version(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDVER_H */
