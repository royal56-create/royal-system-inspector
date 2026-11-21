#include "royal_inspector/system_info.h"
#include <stdio.h>
#include <string.h>
static void usage(const char *p){printf("Royal System Inspector v1.0.0\nUsage: %s <command> [options]\n",p);puts("Commands: overview cpu memory disk processes network diagnostics report --format json|csv|txt help");}
int main(int argc,char **argv){
 rsi_system_info i;const char *c;if(argc<2){usage(argv[0]);return 0;}c=argv[1];if(!strcmp(c,"help")||!strcmp(c,"--help")){usage(argv[0]);return 0;}
 if(rsi_collect_system_info(&i)){fputs("Unable to collect system information.\n",stderr);return 1;}
 if(!strcmp(c,"overview"))rsi_print_overview(&i);else if(!strcmp(c,"cpu"))rsi_print_cpu(&i);else if(!strcmp(c,"memory"))rsi_print_memory(&i);else if(!strcmp(c,"disk"))rsi_print_disk(&i);else if(!strcmp(c,"processes"))rsi_print_processes(&i);else if(!strcmp(c,"network"))rsi_print_network_note();else if(!strcmp(c,"diagnostics"))rsi_print_diagnostics(&i);
 else if(!strcmp(c,"report")){const char *fmt="json";char path[64];int n;for(n=2;n+1<argc;++n)if(!strcmp(argv[n],"--format"))fmt=argv[n+1];if(strcmp(fmt,"json")&&strcmp(fmt,"csv")&&strcmp(fmt,"txt")){fprintf(stderr,"Unsupported format: %s\n",fmt);return 2;}(void)snprintf(path,sizeof path,"system-report.%s",fmt);if(rsi_write_report(&i,fmt,path)){fprintf(stderr,"Could not write %s\n",path);return 1;}printf("Report saved: %s\n",path);}
 else{fprintf(stderr,"Unknown command: %s\n",c);usage(argv[0]);return 2;}return 0;
}
