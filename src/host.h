#ifndef SBS_HOST_H
#define SBS_HOST_H

#ifdef _WIN32
    #define SBS_HOST_OS_WIN
#elif defined(__linux__)
    #define SBS_HOST_OS_LINUX
#endif

typedef enum SbsHostOs {
    SBS_OS_UNK,
    SBS_OS_WIN,
    SBS_OS_LINUX
} SbsHostOs;

typedef enum SbsHostArch {
    SBS_ARCH_UNK,
    SBS_ARCH_X86,
    SBS_ARCH_X86_64,
    SBS_ARCH_ARM,
    SBS_ARCH_ARM64,
} SbsHostArch;

typedef struct SbsHostInfo {
    SbsHostOs os;
    SbsHostArch arch;
    char dir_separator;
} SbsHostInfo;

SbsHostInfo* sbs_host_new(SbsHostOs os, SbsHostArch arch);
SbsHostInfo* sbs_host_get_info(void);
void sbs_host_free(SbsHostInfo *host_info);
SbsHostOs sbs_host_os(void);
SbsHostArch sbs_host_arch(void);
const char* sbs_host_os_to_str(SbsHostOs os);
const char* sbs_host_arch_to_str(SbsHostArch arch);

#endif /* SBS_HOST_H */
