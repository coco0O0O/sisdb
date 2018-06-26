
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_DB_H
#define _STS_DB_H

#include "sts_core.h"
#include "sts_map.h"
#include "sts_table.h"
#include "sts_malloc.h"
#include "sts_time.h"

#define STS_TABLE_MAXLEN 32
#define STS_CODE_MAXLEN  9

#define STS_MAP_DEFINE_FIELD_TYPE   0
#define STS_MAP_DEFINE_DATA_TYPE    1
#define STS_MAP_DEFINE_SCALE        2
#define STS_MAP_DEFINE_OPTION_MODE  3
#define STS_MAP_DEFINE_ZIP_MODE     4
#define STS_MAP_DEFINE_FIELD_METHOD 5

// ����ת��ֻ�ܴӵ͵��ߣ����dayҪתminһ��ʧ��
#define STS_SCALE_NONE    0 // "NONE"  //
#define STS_SCALE_MSEC    1 // "MSEC"  //int64 ��ʽ����ȷ������  
#define STS_SCALE_SECOND  2 // "SECOND"  //int32 time_t��ʽ����ȷ����  
#define STS_SCALE_INDEX   3 // "INDEX"  //int16 0��ʼ�ĵ���������Ӧ���з���
#define STS_SCALE_MIN1    4 // "MIN1"  //int32 time_t��ʽ����ȷ��1����
#define STS_SCALE_MIN5    5 // "MIN5"  //int32 time_t��ʽ ����ȷ��5����
#define STS_SCALE_MIN30   6 // "MIN30"  //int32 time_t��ʽ����ȷ����Сʱ
#define STS_SCALE_DAY     7 // "DAY"  //int32 20170101��ʽ����ȷ����
#define STS_SCALE_MONTH   8 // "MONTH"  //int32 20170101��ʽ����ȷ����


// #define STS_INIT_WAIT    0 // 900��տ��� ��ʼ�ȴ���ʼ��
// #define STS_INIT_WORK    1 // �յ�now��������һ��ֱ�Ӹ�״̬������������µģ��ͳ�ʼ�����״̬ 
// #define STS_INIT_STOP    2 // ����

#pragma pack(push,1)
typedef struct s_sts_map_define{
	const char *key;
	uint8	style;  // ���
	uint16	uid;
	uint32  size;
}s_sts_map_define;

typedef struct s_sts_db {
	s_sts_sds name;  // ���ݿ�����
	s_sts_map_pointer  *db; // һ���ֵ��
	s_sts_map_pointer  *map; // �ؼ��ֲ�ѯ��

	s_sts_struct_list  *trade_time; // [[930,1130],[1300,1500]] ����ʱ�䣬����������

    // int    work_mode;
    // s_sts_struct_list  *work_plans;   // plans-work ��ʱ���� uint16 ������
    // s_sts_time_gap      work_always;  // always-work ѭ�����е�����

    int    save_mode;   // ���̷�ʽ�����ʱ��棬����ָ��ʱ���
    s_sts_struct_list  *save_plans;   // plans-work ��ʱ���� uint16 ������
    s_sts_time_gap      save_always;  // always-work ѭ�����е�����

	// int  save_type;  // ���̷�ʽ�����ʱ��棬����ָ��ʱ���
	// int  save_gaps;  // ���̵ļ������	
	// s_sts_struct_list  *save_plans; // uin16��ʱ�����У��������Ϊgap���ͱ�ʾ��Ϊ��λ�ļ��ʱ��
	s_sts_thread_id_t save_pid;
	s_sts_mutex_t save_mutex;  // saveʱ������

	s_sts_sds conf;    // ����ʱʹ��
	int save_format;   // �����ļ��ķ�ʽ

	s_sts_wait thread_wait; //�߳��ڲ���ʱ����

}s_sts_db;
#pragma pack(pop)


s_sts_db *sts_db_create(char *name);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sts_db_destroy(s_sts_db *);  //�ر�һ�����ݿ�
//ȡ���ݺ�д����
s_sts_table *sts_db_get_table(s_sts_db *, const char *name_); //name -- table name

s_sts_sds sts_db_get_table_info_sds(s_sts_db *);


s_sts_map_define *sts_db_find_map_define(s_sts_db *, const char *name_, uint8 style_);
int sts_db_find_map_uid(s_sts_db *, const char *name_, uint8 style_);


#endif  /* _STS_DB_H */
