#include <stddef.h>
#include <fllib/Cstring.h>
#include <fllib/IO.h>

#include "io.h"
#include "runtime/host.h"

char* sbs_io_to_host_path_realloc(SbsOs os, char *path)
{
    size_t length = strlen(path);

    char host_dir_sep = '\0';
    char dir_sep_to_replace = '\0';

    switch (os)
    {
        case SBS_OS_WIN:
            host_dir_sep = '\\';
            dir_sep_to_replace = '/';
            break;

        case SBS_OS_LINUX:
            host_dir_sep = '/';
            dir_sep_to_replace = '\\';
            break;

        default:
            return NULL;
    }

    for (size_t i=0; i < length; i++)
    {
        if (path[i] != dir_sep_to_replace)
            continue;

        if (os == SBS_OS_LINUX && i < length - 1 && path[i+1] == ' ')
            continue;

        path[i] = host_dir_sep;
    }

    return path;
}

char* sbs_io_to_host_path(SbsOs os, const char *path)
{
    return sbs_io_to_host_path_realloc(os, fl_cstring_dup(path));
}

char* sbs_io_realpath(const char *relpath)
{
    return fl_io_realpath_realloc(sbs_io_to_host_path(sbs_host_os(), relpath));
}
