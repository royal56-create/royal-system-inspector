#include "rsi.h"
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#if defined(__linux__)
#include <sys/sysinfo.h>
#endif
#endif

static void get_arch(char *dst, size_t n) {
#if defined(__x86_64__) || defined(_M_X64)
    snprintf(dst, n, "x86_64");
#elif defined(__aarch64__) || defined(_M_ARM64)
    snprintf(dst, n, "aarch64");
#elif defined(__i386__) || defined(_M_IX86)
    snprintf(dst, n, "x86");
#elif defined(__arm__) || defined(_M_ARM)
    snprintf(dst, n, "arm");
#else
    snprintf(dst, n, "unknown");
#endif
}

int rsi_get_system_info(RsiSystemInfo *info) {
    if (!info) return -1;
    memset(info, 0, sizeof(*info));
#ifdef _WIN32
    snprintf(info->os_name, sizeof(info->os_name), "Windows");
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    info->cpu_cores = (unsigned int)si.dwNumberOfProcessors;
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    if (GlobalMemoryStatusEx(&ms)) {
        info->total_memory_bytes = ms.ullTotalPhys;
        info->available_memory_bytes = ms.ullAvailPhys;
        info->memory_available = 1;
    }
#elif defined(__linux__)
    snprintf(info->os_name, sizeof(info->os_name), "Linux");
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores > 0) info->cpu_cores = (unsigned int)cores;
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        info->total_memory_bytes = (unsigned long long)si.totalram * si.mem_unit;
        info->available_memory_bytes = (unsigned long long)si.freeram * si.mem_unit;
        info->memory_available = 1;
    }
#elif defined(__APPLE__)
    snprintf(info->os_name, sizeof(info->os_name), "macOS");
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores > 0) info->cpu_cores = (unsigned int)cores;
#else
    snprintf(info->os_name, sizeof(info->os_name), "Unknown/portable");
    long cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (cores > 0) info->cpu_cores = (unsigned int)cores;
#endif
    get_arch(info->architecture, sizeof(info->architecture));
    return 0;
}

void rsi_print_system_info(FILE *out, const RsiSystemInfo *info) {
    if (!out || !info) return;
    fprintf(out, "Royal System Inspector v1.0.0\n");
    fprintf(out, "OS: %s\nArchitecture: %s\nCPU cores: %u\n",
            info->os_name, info->architecture, info->cpu_cores);
    if (info->memory_available) {
        fprintf(out, "Memory total: %llu MiB\nMemory available: %llu MiB\n",
                info->total_memory_bytes / (1024ULL * 1024ULL),
                info->available_memory_bytes / (1024ULL * 1024ULL));
    } else {
        fprintf(out, "Memory: not available on this platform\n");
    }
}
