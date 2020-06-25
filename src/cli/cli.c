#include <stdio.h>
#include <stdarg.h>
#include "cli.h"

void sbs_cli_print_header(void)
{
    fprintf(stderr, CLI_HEADER, SBS_VERSION_MAJOR, SBS_VERSION_MINOR, SBS_VERSION_PATCH);
}

void sbs_cli_print_message(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vfprintf(stdout, message, args);
    fprintf(stdout, "\n");
    va_end(args);
}

void sbs_cli_print_error(const char *message, ...)
{
    va_list args;
    va_start(args, message);
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n");
    va_end(args);
}
