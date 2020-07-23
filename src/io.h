#ifndef SBS_IO_H
#define SBS_IO_H

#include "runtime/host.h"

#define SBS_IO_DIR_SEPARATOR FL_IO_DIR_SEPARATOR

char* sbs_io_to_host_path_realloc(SbsHostOs os, char *path);
char* sbs_io_to_host_path(SbsHostOs os, const char *path);
char* sbs_io_realpath(const char *relpath);

#endif /* SBS_IO_H */
