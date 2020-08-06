#include <fllib/Mem.h>
#include <fllib/Cstring.h>
#include "host.h"

#ifdef SBS_HOST_OS_WIN
    #include <windows.h>
    #include <wow64apiset.h>
#elif defined(SBS_HOST_OS_LINUX)
    #define _GNU_SOURCE
    #include <unistd.h>
    #include <stdio.h>    
#endif

SbsHostInfo* sbs_host_new(SbsOs os, SbsArch arch)
{
    SbsHostInfo *host_info = fl_malloc(sizeof(SbsHostInfo));

    host_info->os = os;
    host_info->arch = arch;

    switch (host_info->os)
    {
        case SBS_OS_WIN:
            host_info->dir_separator = '\\';
            break;

        case SBS_OS_LINUX:
            host_info->dir_separator = '/';
            break;

        default:
            host_info->dir_separator = '\0';
            break;
    }

    return host_info;
}

SbsHostInfo* sbs_host_get_info(void)
{
    SbsHostInfo *host_info = fl_malloc(sizeof(SbsHostInfo));

    #ifdef SBS_HOST_OS_WIN
    host_info->os = SBS_OS_WIN;
    host_info->dir_separator = '\\';
    #elif defined(SBS_HOST_OS_LINUX)
    host_info->os = SBS_OS_LINUX;
    host_info->dir_separator = '/';
    #else
    host_info->os = SBS_OS_UNK;
    #endif

    host_info->arch = sbs_host_arch();

    return host_info;
}

void sbs_host_free(SbsHostInfo *host_info)
{
    fl_free(host_info);
}

SbsOs sbs_host_os(void)
{
    #ifdef SBS_HOST_OS_WIN
        return SBS_OS_WIN;
    #elif defined(SBS_HOST_OS_LINUX)
        return SBS_OS_LINUX;
    #else
        return SBS_OS_UNK;
    #endif
}

SbsArch sbs_host_arch(void)
#ifdef SBS_HOST_OS_WIN
{
    SbsArch arch = SBS_ARCH_UNK;

    SYSTEM_INFO sys_info = { 0 };
    GetNativeSystemInfo(&sys_info);

    switch (sys_info.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            arch = SBS_ARCH_X86_64;
            break;

        case PROCESSOR_ARCHITECTURE_ARM64:
            arch = SBS_ARCH_ARM64;
            break;

        case PROCESSOR_ARCHITECTURE_ARM:
            arch = SBS_ARCH_ARM;
            break;

        case PROCESSOR_ARCHITECTURE_INTEL:
            arch = SBS_ARCH_X86;
            break;
    }

    USHORT process_machine = 0;
    USHORT native_machine = 0;
    if (IsWow64Process2(GetCurrentProcess(), &process_machine, &native_machine))
    {
        if (arch == SBS_ARCH_UNK)
        {

        }
    }

    return arch;
}
#elif defined(SBS_HOST_OS_LINUX)
{
    FILE *pf = popen("uname -m", "r");

    if (!pf) 
        return SBS_ARCH_UNK;

    char buffer[1024] = { 0 };
    size_t read = 0;
    if ((read = fread(buffer, 1, 1024, pf)) == 0)
        return SBS_ARCH_UNK;

    if (buffer[read-1] == '\n')
        buffer[read-1] = '\0'; 

    SbsArch arch = SBS_ARCH_UNK;

    if (flm_cstring_equals(buffer, "x86_64"))
    {
        arch = SBS_ARCH_X86_64;
    }
    else if (flm_cstring_equals(buffer, "i386") || flm_cstring_equals(buffer, "i686"))
    {
        arch = SBS_ARCH_X86;
    }
    // TODO: Fix arm/arm64 arch identifier

    pclose(pf);

    return arch;
}
#else
{
    return SBS_ARCH_UNK;
}
#endif

const char* sbs_os_to_str(SbsOs os)
{
    switch (os)
    {
        case SBS_OS_WIN:
            return "win";

        case SBS_OS_LINUX:
            return "linux";

        default:
            return "unknown";
    }
}

const char* sbs_arch_to_str(SbsArch arch)
{
    switch (arch)
    {
        case SBS_ARCH_X86:
            return "x86";

        case SBS_ARCH_X86_64:
            return "x86_64";

        // TODO: Fix arm arch identifier
        case SBS_ARCH_ARM:
            return "arm";

        // TODO: Fix arm64 arch identifier
        case SBS_ARCH_ARM64:
            return "arm64";

        default:
            return "unknown";
    }
}

SbsArch sbs_arch_from_str(const char *arch)
{
    if (flm_cstring_equals(arch, "x86"))
        return SBS_ARCH_X86;

    if (flm_cstring_equals(arch, "x86_64"))
        return SBS_ARCH_X86_64;

    // TODO: Fix arm arch identifier
    if (flm_cstring_equals(arch, "arm"))
        return SBS_ARCH_ARM;

    // TODO: Fix arm64 arch identifier
    if (flm_cstring_equals(arch, "arm64"))
        return SBS_ARCH_ARM64;

    return SBS_ARCH_UNK;
}
