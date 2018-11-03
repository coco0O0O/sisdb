#ifndef _OS_TIME_H
#define _OS_TIME_H

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
// #include <string.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <ctype.h>

#define set_time_get_day gettimeofday

void sis_time_getgm(struct tm *m_, const time_t* t_);
void sis_localtime(struct tm *m, const time_t* t);

//获取当前秒数
time_t sis_time_get_now(); 
//获得当前毫秒数
unsigned long long sis_time_get_now_msec(); 
// 检查tt用当前时间替换
void sis_time_check(time_t tt_, struct tm *ptm_);
//睡眠单位毫秒
void sis_sleep(int msec);

#endif //_OS_TIME_H