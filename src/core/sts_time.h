#ifndef _STS_TIME_H
#define _STS_TIME_H

#include <sts_core.h>
#include <sts_str.h>

int sts_time_get_iyear(time_t ttime); //2015
int sts_time_get_imonth(time_t ttime); //201510
int sts_time_get_idate(time_t ttime); //20151012
int sts_time_get_itime(time_t ttime); //103020
int sts_time_get_iminute(time_t ttime); //1030
int sts_time_get_isec(time_t ttime); // 20
int sts_time_get_showtime(time_t ttime); //0212103020 ����ʱ��

short sts_time_get_offset_iminute(short nMin, int offsec);
time_t sts_time_get_offset_time(time_t curTime, int offsec);  //���ӻ�����룬����time_t
int sts_time_get_iminute_offset_i(int begin, int end); //930-1130 
int sts_time_get_iminute_offset_time(time_t tstart, time_t tend);// �ж��м���������
int sts_time_get_iminute_minnum(int source, int minnum);  //���ӻ���ٷ���  900,-5 --> 855

time_t sts_time_make_time(int tdate, int ttime);
int sts_time_get_week_ofday(int today);// �ж����ܼ� [0,6]
int sts_time_get_month_ofday(int today);// �ж��Ǽ���[0,11]
int sts_time_get_dayoffset_ofday(int tstart, int tend);// �ж��м�������
int sts_time_next_work_day(int today_, int offset_);//������ĩ

bool sts_time_str_is_date(char* date); //�ж��ַ����ǲ�������20150212

void sts_time_format_minute(char * out_, size_t olen_, time_t tt_); //"930"
void sts_time_format_date(char * out_, size_t olen_, time_t tt_); //"20150912"
void sts_time_format_datetime(char * out_, size_t olen_, time_t tt_); //"20150912103000"

int sts_time_get_minute_from_shortstr(char* time);//"12:30" => 1230
int sts_time_get_itime_from_str(char* time);//"12:30:38" => 123038
int sts_time_get_idate_from_str(const char* time);//"20150212" => 20150212
int sts_time_get_time_from_longstr(char* datetime, int* nDate, int* nTime); //"2015-10-20 12:30:38" => 20151020,123038


typedef struct s_sts_time_delay {
	bool is_busy;
	unsigned int m_msec_i;
	unsigned int usesd_msec;
	unsigned long long start_msec;
} s_sts_time_delay;

s_sts_time_delay *sts_delay_create(unsigned int msec);
void sts_delay_busy(s_sts_time_delay *m);
void sts_delay_destroy(s_sts_time_delay *m);

typedef struct s_sts_time_pair{
	uint16	first;  // ��λ����
	uint16	second; // ��λ����
}s_sts_time_pair;

typedef struct s_sts_time_gap{
	uint16	start; // ��λ���� ���Ϊ0�Ͳ��ж�
	uint16	stop;  // ��λ���� ���Ϊ0�Ͳ��ж�
	uint32	delay; // ���������
}s_sts_time_gap;

#endif //_STS_TIME_H