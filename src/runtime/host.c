#include <fllib/Mem.h>
#include "host.h"

#ifdef SBS_HOST_OS_WIN
    #include <windows.h>
    #include <wow64apiset.h>
#elif defined(SBS_HOST_OS_LINUX)

#endif

SbsHostInfo* sbs_host_new(SbsHostOs os, SbsHostArch arch)
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

SbsHostOs sbs_host_os(void)
{
    #ifdef SBS_HOST_OS_WIN
        return SBS_OS_WIN;
    #elif defined(SBS_HOST_OS_LINUX)
        return SBS_OS_LINUX;
    #else
        return SBS_OS_UNK;
    #endif
}

SbsHostArch sbs_host_arch(void)
#ifdef SBS_HOST_OS_WIN
{
    SbsHostArch arch = SBS_ARCH_UNK;

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
    // TODO:
    return SBS_ARCH_UNK;
}
#else
{
    return SBS_ARCH_UNK;
}
#endif

const char* sbs_host_os_to_str(SbsHostOs os)
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

const char* sbs_host_arch_to_str(SbsHostArch arch)
{
    switch (arch)
    {
        case SBS_ARCH_X86:
            return "x86";

        case SBS_ARCH_X86_64:
            return "x86_64";

        case SBS_ARCH_ARM:
            return "arm";

        case SBS_ARCH_ARM64:
            return "arm64";

        default:
            return "unknown";
    }
}
