#include "royal_inspector/system_info.h"
#include <assert.h>
int main(void){rsi_system_info i;assert(rsi_collect_system_info(&i)==0);assert(i.os_name[0]!='\0');assert(i.architecture[0]!='\0');assert(i.logical_cpus>=1);return 0;}
