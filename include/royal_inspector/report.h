#ifndef RSI_REPORT_H
#define RSI_REPORT_H
#include "royal_inspector/system_info.h"
int rsi_write_report(const rsi_system_info *info,const char *format,const char *path);
#endif
