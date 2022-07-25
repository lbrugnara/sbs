#include <stdio.h>
#include "cli.h"
#include "../version.h"
#include "../result.h"

#ifndef SBS_BUILD_HOST_OS
#define SBS_BUILD_HOST_OS "(unk)"
#endif

#ifndef SBS_BUILD_HOST_ARCH
#define SBS_BUILD_HOST_ARCH "(unk)"
#endif

#ifndef SBS_BUILD_OS
#define SBS_BUILD_OS "(unk)"
#endif

#ifndef SBS_BUILD_ARCH
#define SBS_BUILD_ARCH "(unk)"
#endif

#ifndef SBS_BUILD_COMPILER
#define SBS_BUILD_COMPILER "(unk)"
#endif

#ifndef SBS_BUILD_LINKER
#define SBS_BUILD_LINKER "(unk)"
#endif

#ifndef SBS_BUILD_CONFIGURATION
#define SBS_BUILD_CONFIGURATION "(unk)"
#endif


static const char *version_message = 
    CLI_HEADER
    "Build information:                                                             \n"
    "   - Host OS: %s                                                               \n"
    "   - Host architecture: %s                                                     \n"
    "   - Target OS: %s                                                             \n"
    "   - Target architecture: %s                                                   \n"
    "   - Toolchain:                                                                \n"
    "       - Compiler: %s                                                          \n"
    "       - Linker: %s                                                            \n"
    "   - Configuration: %s                                                         \n"
    "                                                                                 "
;

SbsResult sbs_command_version(int argc, char **argv, char **env, size_t argv_offset)
{
    sbs_cli_print_message(version_message, 
        SBS_VERSION_MAJOR, 
        SBS_VERSION_MINOR, 
        SBS_VERSION_PATCH,
        SBS_BUILD_HOST_OS,
        SBS_BUILD_HOST_ARCH,
        SBS_BUILD_OS,
        SBS_BUILD_ARCH,
        SBS_BUILD_COMPILER,
        SBS_BUILD_LINKER,
        SBS_BUILD_CONFIGURATION
    );
    return SBS_RES_OK;
}
