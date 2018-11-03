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

//��ȡ��ǰ����
time_t sis_time_get_now(); 
//��õ�ǰ������
unsigned long long sis_time_get_now_msec(); 
// ���tt�õ�ǰʱ���滻
void sis_time_check(time_t tt_, struct tm *ptm_);
//˯�ߵ�λ����
void sis_sleep(int msec);

#endif //_OS_TIME_H