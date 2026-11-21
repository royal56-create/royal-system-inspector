#include "rsi.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <sys/statvfs.h>
#endif

int rsi_print_disk_info(const char *path, FILE *out) {
    if (!path || !out) return -1;
#ifdef _WIN32
    ULARGE_INTEGER available, total, free_bytes;
    if (!GetDiskFreeSpaceExA(path, &available, &total, &free_bytes)) return -1;
    fprintf(out, "Disk path: %s\nTotal: %llu MiB\nAvailable to caller: %llu MiB\n",
            path, (unsigned long long)(total.QuadPart / (1024ULL*1024ULL)),
            (unsigned long long)(available.QuadPart / (1024ULL*1024ULL)));
#else
    struct statvfs v;
    if (statvfs(path, &v) != 0) return -1;
    unsigned long long unit = (unsigned long long)v.f_frsize;
    fprintf(out, "Disk path: %s\nTotal: %llu MiB\nAvailable: %llu MiB\n",
            path, (unsigned long long)v.f_blocks * unit / (1024ULL*1024ULL),
            (unsigned long long)v.f_bavail * unit / (1024ULL*1024ULL));
#endif
    return 0;
}
