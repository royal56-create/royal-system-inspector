#ifndef RSI_H
#define RSI_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char os_name[64];
    char architecture[64];
    unsigned int cpu_cores;
    unsigned long long total_memory_bytes;
    unsigned long long available_memory_bytes;
    int memory_available;
} RsiSystemInfo;

typedef struct {
    unsigned long long bytes;
    unsigned long long lines;
    unsigned long long words;
} RsiFileStats;

int rsi_get_system_info(RsiSystemInfo *info);
int rsi_analyze_file(const char *path, RsiFileStats *stats);
int rsi_hash_file_fnv1a64(const char *path, uint64_t *hash_out);
int rsi_print_disk_info(const char *path, FILE *out);
void rsi_print_system_info(FILE *out, const RsiSystemInfo *info);
void rsi_print_usage(FILE *out, const char *program);

#endif
