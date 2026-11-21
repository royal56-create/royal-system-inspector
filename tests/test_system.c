#include "royal_inspector/system_info.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check_report_safety(void) {
    rsi_system_info sample = {0};
    char buffer[2048];
    FILE *file;
    size_t count;

    strcpy(sample.os_name, "Test OS");
    strcpy(sample.hostname, "host\"name\nnext");
    strcpy(sample.architecture, "test-arch");
    strcpy(sample.cpu_model, "sample\\cpu");
    sample.logical_cpus = 4;

    remove("test-report.json");
    assert(rsi_write_report(&sample, "json", "test-report.json") == 0);
    assert(rsi_write_report(&sample, "json", "test-report.json") != 0);
    file = fopen("test-report.json", "r");
    assert(file != NULL);
    count = fread(buffer, 1, sizeof buffer - 1, file);
    buffer[count] = '\0';
    fclose(file);
    assert(strstr(buffer, "host\\\"name\\nnext") != NULL);
    assert(strstr(buffer, "sample\\\\cpu") != NULL);
    remove("test-report.json");

    remove("test-report.csv");
    assert(rsi_write_report(&sample, "csv", "test-report.csv") == 0);
    file = fopen("test-report.csv", "r");
    assert(file != NULL);
    count = fread(buffer, 1, sizeof buffer - 1, file);
    buffer[count] = '\0';
    fclose(file);
    assert(strstr(buffer, "\"host\"\"name") != NULL);
    remove("test-report.csv");
}

int main(void) {
    rsi_system_info i;
    assert(rsi_collect_system_info(&i) == 0);
    assert(i.os_name[0] != '\0');
    assert(i.architecture[0] != '\0');
    assert(i.logical_cpus >= 1);
    check_report_safety();
    return 0;
}
