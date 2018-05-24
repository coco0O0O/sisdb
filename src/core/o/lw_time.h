#ifndef __LW_TIME_H
#define __LW_TIME_H

#ifdef _MSC_VER
#include <winsock2.h>
#include <time.h>
#include <stdbool.h>
#pragma warning(disable: 4101 4996)
#else
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#endif

#include "lw_public.h"

#ifdef _MSC_VER
int gettimeofday(struct timeval *tp, void *tzp);
#endif

void dp_gmtime(struct tm *m, const time_t* t);
void dp_localtime(struct tm *m, const time_t* t);

time_t dp_nowtime(); //��ȡ��ǰ����
unsigned long long dp_nowtime_msec();  //��õ�ǰ������

int dp_get_iyear(time_t ttime); //2015
int dp_get_imonth(time_t ttime); //201510
int dp_get_idate(time_t ttime); //20151012
int dp_get_itime(time_t ttime); //103020
int dp_get_iminute(time_t ttime); //1030
int dp_get_isec(time_t ttime); // 20
int dp_get_showtime(time_t ttime); //0212103020 ����ʱ��

short dp_get_offset_iminute(short nMin, int offsec);
time_t dp_get_offset_time(time_t curTime, int offsec);  //���ӻ�����룬����time_t
int dp_get_iminute_offset_i(int begin, int end); //930-1130 
int dp_get_iminute_offset_time(time_t tstart, time_t tend);// �ж��м���������
int dp_get_iminute_minnum(int source, int minnum);  //���ӻ���ٷ���  900,-5 --> 855

time_t dp_make_time(int tdate, int ttime);
int dp_get_week_ofday(int today);// �ж����ܼ� [0,6]
int dp_get_month_ofday(int today);// �ж��Ǽ���[0,11]
int dp_get_dayoffset_ofday(int tstart, int tend);// �ж��м�������
int dp_next_work_day(int today_, int offset_);//������ĩ

bool dp_str_is_date(char* date); //�ж��ַ����ǲ�������20150212

void format_minute(char * curTime, time_t ttime); //"930"
void format_date(char * curTime, time_t ttime); //"20150912"
void format_datetime(char * curTime, time_t ttime); //"20150912103000"

int get_minute_from_shortstr(char* time);//"12:30" => 1230
int get_itime_from_str(char* time);//"12:30:38" => 123038
int get_idate_from_str(const char* time);//"20150212" => 20150212
int get_time_from_longstr(char* datetime, int* nDate, int* nTime); //"2015-10-20 12:30:38" => 20151020,123038

void dp_sleep(int msec);//��λ����

typedef struct s_delay {
	bool is_busy;
	unsigned int m_msec_i;
	unsigned int usesd_msec;
	unsigned long long start_msec;
} s_delay;

s_delay *delay_create(unsigned int msec);
void delay_busy(s_delay *m);
void delay_destroy(s_delay *m);

#endif //__LW_TIME_H