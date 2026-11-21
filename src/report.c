#include "royal_inspector/system_info.h"
#include <stdio.h>
#include <string.h>

static int write_json_string(FILE *f, const char *value) {
    const unsigned char *p = (const unsigned char *)(value ? value : "");
    if (fputc('"', f) == EOF) return -1;
    for (; *p; ++p) {
        switch (*p) {
            case '"': if (fputs("\\\"", f) == EOF) return -1; break;
            case '\\': if (fputs("\\\\", f) == EOF) return -1; break;
            case '\b': if (fputs("\\b", f) == EOF) return -1; break;
            case '\f': if (fputs("\\f", f) == EOF) return -1; break;
            case '\n': if (fputs("\\n", f) == EOF) return -1; break;
            case '\r': if (fputs("\\r", f) == EOF) return -1; break;
            case '\t': if (fputs("\\t", f) == EOF) return -1; break;
            default:
                if (*p < 0x20) {
                    if (fprintf(f, "\\u%04x", (unsigned int)*p) < 0) return -1;
                } else if (fputc(*p, f) == EOF) return -1;
        }
    }
    return fputc('"', f) == EOF ? -1 : 0;
}

static int write_csv_field(FILE *f, const char *value) {
    const char *p = value ? value : "";
    if (fputc('"', f) == EOF) return -1;
    while (*p) {
        if (*p == '"' && fputc('"', f) == EOF) return -1;
        if (fputc((unsigned char)*p++, f) == EOF) return -1;
    }
    return fputc('"', f) == EOF ? -1 : 0;
}

int rsi_write_report(const rsi_system_info *i, const char *fmt, const char *path) {
    FILE *f;
    int failed = 0;
    if (!i || !fmt || !path) return -1;

    /* Exclusive creation prevents silently destroying an earlier report. */
    f = fopen(path, "wx");
    if (!f) return -1;

    if (!strcmp(fmt, "json")) {
        failed |= fputs("{\n \"os\": ", f) == EOF || write_json_string(f, i->os_name) != 0;
        failed |= fputs(",\n \"hostname\": ", f) == EOF || write_json_string(f, i->hostname) != 0;
        failed |= fputs(",\n \"architecture\": ", f) == EOF || write_json_string(f, i->architecture) != 0;
        failed |= fputs(",\n \"cpu_model\": ", f) == EOF || write_json_string(f, i->cpu_model) != 0;
        failed |= fprintf(f, ",\n \"logical_cpus\": %u,\n \"total_memory_bytes\": %llu,\n \"available_memory_bytes\": %llu,\n \"total_disk_bytes\": %llu,\n \"available_disk_bytes\": %llu,\n \"process_count\": %u\n}\n",
            i->logical_cpus, (unsigned long long)i->total_memory_bytes,
            (unsigned long long)i->available_memory_bytes, (unsigned long long)i->total_disk_bytes,
            (unsigned long long)i->available_disk_bytes, i->process_count) < 0;
    } else if (!strcmp(fmt, "csv")) {
        failed |= fputs("os,hostname,architecture,cpu_model,logical_cpus,total_memory_bytes,available_memory_bytes,total_disk_bytes,available_disk_bytes,process_count\n", f) == EOF;
        failed |= write_csv_field(f, i->os_name) != 0 || fputc(',', f) == EOF;
        failed |= write_csv_field(f, i->hostname) != 0 || fputc(',', f) == EOF;
        failed |= write_csv_field(f, i->architecture) != 0 || fputc(',', f) == EOF;
        failed |= write_csv_field(f, i->cpu_model) != 0 || fputc(',', f) == EOF;
        failed |= fprintf(f, "%u,%llu,%llu,%llu,%llu,%u\n", i->logical_cpus,
            (unsigned long long)i->total_memory_bytes, (unsigned long long)i->available_memory_bytes,
            (unsigned long long)i->total_disk_bytes, (unsigned long long)i->available_disk_bytes,
            i->process_count) < 0;
    } else if (!strcmp(fmt, "txt")) {
        failed |= fprintf(f, "Royal System Inspector\nOS: %s\nHostname: %s\nArchitecture: %s\nCPU: %s\nLogical CPUs: %u\n",
            i->os_name, i->hostname, i->architecture, i->cpu_model, i->logical_cpus) < 0;
    } else {
        failed = 1;
    }

    if (fclose(f) != 0) failed = 1;
    if (failed) {
        remove(path);
        return -1;
    }
    return 0;
}
