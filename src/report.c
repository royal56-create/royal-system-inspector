#include "royal_inspector/system_info.h"
#include <stdio.h>
#include <string.h>
int rsi_write_report(const rsi_system_info *i,const char *fmt,const char *path){
 FILE *f;if(!i||!fmt||!path)return -1;f=fopen(path,"w");if(!f)return -1;
 if(!strcmp(fmt,"json"))fprintf(f,"{\n \"os\": \"%s\",\n \"hostname\": \"%s\",\n \"architecture\": \"%s\",\n \"cpu_model\": \"%s\",\n \"logical_cpus\": %u,\n \"total_memory_bytes\": %llu,\n \"available_memory_bytes\": %llu,\n \"total_disk_bytes\": %llu,\n \"available_disk_bytes\": %llu,\n \"process_count\": %u\n}\n",i->os_name,i->hostname,i->architecture,i->cpu_model,i->logical_cpus,(unsigned long long)i->total_memory_bytes,(unsigned long long)i->available_memory_bytes,(unsigned long long)i->total_disk_bytes,(unsigned long long)i->available_disk_bytes,i->process_count);
 else if(!strcmp(fmt,"csv"))fprintf(f,"os,hostname,architecture,cpu_model,logical_cpus,total_memory_bytes,available_memory_bytes,total_disk_bytes,available_disk_bytes,process_count\n\"%s\",\"%s\",\"%s\",\"%s\",%u,%llu,%llu,%llu,%llu,%u\n",i->os_name,i->hostname,i->architecture,i->cpu_model,i->logical_cpus,(unsigned long long)i->total_memory_bytes,(unsigned long long)i->available_memory_bytes,(unsigned long long)i->total_disk_bytes,(unsigned long long)i->available_disk_bytes,i->process_count);
 else if(!strcmp(fmt,"txt"))fprintf(f,"Royal System Inspector\nOS: %s\nHostname: %s\nArchitecture: %s\nCPU: %s\nLogical CPUs: %u\n",i->os_name,i->hostname,i->architecture,i->cpu_model,i->logical_cpus);
 else{fclose(f);return -1;}return fclose(f)==0?0:-1;
}
