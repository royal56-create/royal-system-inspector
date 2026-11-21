#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#ifdef __APPLE__
#define _DARWIN_C_SOURCE 1
#endif
#include "royal_inspector/system_info.h"
#include <stdio.h>
#include <string.h>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#else
#include <dirent.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <unistd.h>
#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#endif
#endif
static void cp(char *d,size_t n,const char *s){if(n)snprintf(d,n,"%s",s?s:"Unknown");}
#ifndef _WIN32
static void cpu_model(char *out,size_t n){
 FILE *f=fopen("/proc/cpuinfo","r"); char line[256]; cp(out,n,"Unavailable"); if(!f)return;
 while(fgets(line,sizeof line,f)) if(!strncmp(line,"model name",10)||!strncmp(line,"Hardware",8)){
  char *p=strchr(line,':'); if(p){++p;while(*p==' '||*p=='\t')++p;p[strcspn(p,"\r\n")]=0;cp(out,n,p);break;}}
 fclose(f);
}
static void memory(rsi_system_info *i){
 FILE *f=fopen("/proc/meminfo","r");char key[64],unit[16];unsigned long long v;if(!f)return;
 while(fscanf(f,"%63s %llu %15s",key,&v,unit)==3){
  if(!strcmp(key,"MemTotal:"))i->total_memory_bytes=(uint64_t)v*1024U;
  if(!strcmp(key,"MemAvailable:")){i->available_memory_bytes=(uint64_t)v*1024U;break;}}
 fclose(f);i->has_memory=i->total_memory_bytes>0;
}
static unsigned int processes(void){
 DIR *d=opendir("/proc");struct dirent *e;unsigned int n=0;if(!d)return 0;
 while((e=readdir(d))){const char *p=e->d_name;int digits=*p!=0;while(*p){if(*p<'0'||*p>'9'){digits=0;break;}++p;}if(digits)++n;}
 closedir(d);return n;
}
static unsigned int logical_cpus(void){
#ifdef __APPLE__
 int count=0;size_t size=sizeof count;
 if(sysctlbyname("hw.logicalcpu",&count,&size,NULL,0)==0&&count>0)return (unsigned int)count;
 return 1;
#else
 long count=sysconf(_SC_NPROCESSORS_ONLN);
 return count>0?(unsigned int)count:1;
#endif
}
#endif
int rsi_collect_system_info(rsi_system_info *i){
 if(!i)return -1;memset(i,0,sizeof *i);
#ifdef _WIN32
 SYSTEM_INFO si;MEMORYSTATUSEX m;char host[128]="Unknown";DWORD hn=sizeof host;
 ULARGE_INTEGER freeb,totalb;HANDLE snap;PROCESSENTRY32 pe;unsigned int count=0;
 GetSystemInfo(&si);i->logical_cpus=si.dwNumberOfProcessors?si.dwNumberOfProcessors:1;
 cp(i->architecture,sizeof i->architecture,si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64?"x86_64":si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_ARM64?"ARM64":si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL?"x86":"Unknown");
 cp(i->os_name,sizeof i->os_name,"Windows");if(GetComputerNameA(host,&hn))cp(i->hostname,sizeof i->hostname,host);
 cp(i->cpu_model,sizeof i->cpu_model,"Not queried on Windows in this release");
 ZeroMemory(&m,sizeof m);m.dwLength=sizeof m;if(GlobalMemoryStatusEx(&m)){i->total_memory_bytes=m.ullTotalPhys;i->available_memory_bytes=m.ullAvailPhys;i->has_memory=1;}
 if(GetDiskFreeSpaceExA("C:\\",&freeb,&totalb,NULL)){i->total_disk_bytes=totalb.QuadPart;i->available_disk_bytes=freeb.QuadPart;i->has_disk=1;}
 snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(snap!=INVALID_HANDLE_VALUE){ZeroMemory(&pe,sizeof pe);pe.dwSize=sizeof pe;if(Process32First(snap,&pe))do{++count;}while(Process32Next(snap,&pe));CloseHandle(snap);i->process_count=count;i->has_process_count=1;}
#else
 struct utsname u;char host[128]="Unknown";struct statvfs v;
 if(uname(&u)==0){cp(i->os_name,sizeof i->os_name,u.sysname);cp(i->architecture,sizeof i->architecture,u.machine);}else{cp(i->os_name,sizeof i->os_name,"Unix-like");cp(i->architecture,sizeof i->architecture,"Unknown");}
 if(gethostname(host,sizeof host-1)==0)cp(i->hostname,sizeof i->hostname,host);
 cpu_model(i->cpu_model,sizeof i->cpu_model);i->logical_cpus=logical_cpus();
 memory(i);if(statvfs("/",&v)==0){i->total_disk_bytes=(uint64_t)v.f_blocks*v.f_frsize;i->available_disk_bytes=(uint64_t)v.f_bavail*v.f_frsize;i->has_disk=1;}
 {FILE *f=fopen("/proc/uptime","r");double s;if(f){if(fscanf(f,"%lf",&s)==1&&s>=0){i->uptime_seconds=(unsigned long)s;i->has_uptime=1;}fclose(f);}}
 i->process_count=processes();i->has_process_count=1;
#endif
 return 0;
}
static void bytes(const char *label,uint64_t n){const char *u[]={"B","KiB","MiB","GiB","TiB"};double x=(double)n;unsigned k=0;while(x>=1024&&k<4){x/=1024;++k;}printf("%-22s %.2f %s\n",label,x,u[k]);}
void rsi_print_overview(const rsi_system_info *i){
 puts("Royal System Inspector\n======================");printf("OS: %s\nHostname: %s\nArchitecture: %s\nCPU: %s\nLogical CPUs: %u\n",i->os_name,i->hostname,i->architecture,i->cpu_model,i->logical_cpus);
 if(i->has_uptime)printf("Uptime: %lu seconds\n",i->uptime_seconds);if(i->has_process_count)printf("Processes: %u\n",i->process_count);
 if(i->has_memory){bytes("Total memory:",i->total_memory_bytes);bytes("Available memory:",i->available_memory_bytes);}if(i->has_disk){bytes("Root/C total:",i->total_disk_bytes);bytes("Root/C available:",i->available_disk_bytes);}
}
void rsi_print_cpu(const rsi_system_info *i){printf("CPU model: %s\nArchitecture: %s\nLogical processors: %u\n",i->cpu_model,i->architecture,i->logical_cpus);puts("CPU utilization and temperature sensors are not implemented.");}
void rsi_print_memory(const rsi_system_info *i){if(!i->has_memory){puts("Memory data unavailable.");return;}bytes("Total:",i->total_memory_bytes);bytes("Available:",i->available_memory_bytes);bytes("Used estimate:",i->total_memory_bytes-i->available_memory_bytes);}
void rsi_print_disk(const rsi_system_info *i){if(!i->has_disk){puts("Disk data unavailable.");return;}bytes("Total root/C:",i->total_disk_bytes);bytes("Available:",i->available_disk_bytes);bytes("Used:",i->total_disk_bytes-i->available_disk_bytes);puts("Only Linux root filesystem or Windows C: drive is reported.");}
void rsi_print_processes(const rsi_system_info *i){if(i->has_process_count)printf("Visible process count: %u\n",i->process_count);else puts("Process count unavailable.");puts("Process details and termination are not provided.");}
void rsi_print_network_note(void){puts("Network interface enumeration is not implemented in this release. No traffic is captured.");}
void rsi_print_diagnostics(const rsi_system_info *i){
 if(i->has_memory&&i->total_memory_bytes){double p=100.0*(double)(i->total_memory_bytes-i->available_memory_bytes)/(double)i->total_memory_bytes;printf("Memory used: %.1f%% [%s]\n",p,p>=90?"HIGH":p>=75?"NOTICE":"OK");}else puts("Memory unavailable; not evaluated.");
 if(i->has_disk&&i->total_disk_bytes){double p=100.0*(double)(i->total_disk_bytes-i->available_disk_bytes)/(double)i->total_disk_bytes;printf("Root/C disk used: %.1f%% [%s]\n",p,p>=90?"HIGH":p>=80?"NOTICE":"OK");}else puts("Disk unavailable; not evaluated.");puts("Informational only; no system settings are changed.");
}
