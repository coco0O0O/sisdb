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

void sts_time_getgm(struct tm *m_, const time_t* t_);
void sts_localtime(struct tm *m, const time_t* t);

//��ȡ��ǰ����
time_t sts_time_get_now(); 
//��õ�ǰ������
unsigned long long sts_time_get_now_msec(); 
// ���tt�õ�ǰʱ���滻
void sts_time_check(time_t tt_, struct tm *ptm_);
//˯�ߵ�λ����
void sts_sleep(int msec);

#endif //_OS_TIME_H