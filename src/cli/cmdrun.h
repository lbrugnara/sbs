#ifndef SBS_CMDRUN_H
#define SBS_CMDRUN_H

#include <stddef.h>
#include "../result.h"

SbsResult sbs_command_run(int argc, char **argv, char **env, size_t argv_offset);

#endif /* SBS_CMDRUN_H */
