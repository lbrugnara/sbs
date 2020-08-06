#ifndef SBS_RUNTIME_HOST_H
#define SBS_RUNTIME_HOST_H

#ifdef _WIN32
    #define SBS_HOST_OS_WIN
#elif defined(__linux__)
    #define SBS_HOST_OS_LINUX
#endif

typedef enum SbsOs {
    SBS_OS_UNK,
    SBS_OS_WIN,
    SBS_OS_LINUX
} SbsOs;

typedef enum SbsArch {
    SBS_ARCH_UNK,
    SBS_ARCH_X86,
    SBS_ARCH_X86_64,
    SBS_ARCH_ARM,
    SBS_ARCH_ARM64,
} SbsArch;

typedef struct SbsHostInfo {
    SbsOs os;
    SbsArch arch;
    char dir_separator;
} SbsHostInfo;

SbsHostInfo* sbs_host_new(SbsOs os, SbsArch arch);
SbsHostInfo* sbs_host_get_info(void);
void sbs_host_free(SbsHostInfo *host_info);
SbsOs sbs_host_os(void);
SbsArch sbs_host_arch(void);
const char* sbs_os_to_str(SbsOs os);
const char* sbs_arch_to_str(SbsArch arch);
SbsArch sbs_arch_from_str(const char *arch);

#endif /* SBS_RUNTIME_HOST_H */
