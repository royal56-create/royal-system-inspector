#ifndef RSI_SYSTEM_INFO_H
#define RSI_SYSTEM_INFO_H
#include <stdint.h>
typedef struct {
 char os_name[96], hostname[128], architecture[64], cpu_model[160];
 unsigned int logical_cpus, process_count;
 uint64_t total_memory_bytes, available_memory_bytes, total_disk_bytes, available_disk_bytes;
 unsigned long uptime_seconds;
 int has_memory, has_disk, has_uptime, has_process_count;
} rsi_system_info;
int rsi_collect_system_info(rsi_system_info *info);
void rsi_print_overview(const rsi_system_info *info);
void rsi_print_cpu(const rsi_system_info *info);
void rsi_print_memory(const rsi_system_info *info);
void rsi_print_disk(const rsi_system_info *info);
void rsi_print_processes(const rsi_system_info *info);
void rsi_print_network_note(void);
void rsi_print_diagnostics(const rsi_system_info *info);
int rsi_write_report(const rsi_system_info *info, const char *format, const char *path);
#endif
