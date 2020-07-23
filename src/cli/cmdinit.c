#include <fllib/IO.h>

#include "cmdinit.h"

#include "cli.h"
#include "../result.h"

SbsResult sbs_command_init(int argc, char **argv, char **env)
{
    sbs_cli_print_header();

    if (!fl_io_file_exists(".sbs"))
    {
        if (!fl_io_dir_create(".sbs"))
        {
            sbs_cli_print_error("Could not create the .sbs folder. Check your permissions");
            return SBS_RES_ERROR;
        }
    }

    // Default filename
    const char *build_file = ".sbs/build.sbs";

    // If it exists, we return an error
    if (fl_io_file_exists(build_file))
    {
        sbs_cli_print_error("sbs has already been initialized");
        return SBS_RES_ERROR;
    }

    // Just a comment
    if (!fl_io_file_write_all_text(build_file, "# Empty"))
    {
        sbs_cli_print_error("Could not initialize sbs.");
        return SBS_RES_ERROR;
    }

    // Notify everything it's ok
    sbs_cli_print_message("sbs has been initialized. Start editing your %s file", build_file);

    return SBS_RES_OK;
}
