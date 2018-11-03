
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SIS_DB_H
#define _SIS_DB_H

#include "sis_core.h"
#include "sis_map.h"
#include "sis_malloc.h"
#include "sis_time.h"
#include "sis_list.h"

#define SIS_TABLE_MAXLEN 32
#define SIS_CODE_MAXLEN  9

#define SIS_MAP_DEFINE_FIELD_TYPE   0
#define SIS_MAP_DEFINE_DATA_TYPE    1
#define SIS_MAP_DEFINE_SCALE        2
#define SIS_MAP_DEFINE_OPTION_MODE  3
#define SIS_MAP_DEFINE_ZIP_MODE     4
#define SIS_MAP_DEFINE_FIELD_METHOD 5

// ����ת��ֻ�ܴӵ͵��ߣ����dayҪתminһ��ʧ��
#define SIS_SCALE_NONE    0 // "NONE"  //
#define SIS_SCALE_MSEC    1 // "MSEC"  //int64 ��ʽ����ȷ������  
#define SIS_SCALE_SECOND  2 // "SECOND"  //int32 time_t��ʽ����ȷ����  
#define SIS_SCALE_INDEX   3 // "INDEX"  //int16 0��ʼ�ĵ���������Ӧ���з���
#define SIS_SCALE_MIN1    4 // "MIN1"  //int32 time_t��ʽ����ȷ��1����
#define SIS_SCALE_MIN5    5 // "MIN5"  //int32 time_t��ʽ ����ȷ��5����
#define SIS_SCALE_MIN30   6 // "MIN30"  //int32 time_t��ʽ����ȷ����Сʱ
#define SIS_SCALE_DAY     7 // "DAY"  //int32 20170101��ʽ����ȷ����
#define SIS_SCALE_MONTH   8 // "MONTH"  //int32 20170101��ʽ����ȷ����

/////////////////////////////////////////////////////////
//  ���ݿ����ݲ�����޸�ģʽ
/////////////////////////////////////////////////////////
#define SIS_OPTION_ALWAYS      0  // �����ж�ֱ��׷��
#define SIS_OPTION_TIME        1  // ���ʱ��ڵ��ظ��͸����ϵ����ݣ����ظ��ͷŶ�Ӧλ��
#define SIS_OPTION_VOL         2  // ���ɽ������ɽ������Ӿ�д��
#define SIS_OPTION_CODE        4  // �����ж�ֱ��׷��
#define SIS_OPTION_SORT        8  // ��ʱ������ͬһ��ʱ������ж������
#define SIS_OPTION_NONE        16  // ���ܲ���ֻ���޸� һ�㲻��

/////////////////////////////////////////////////////////
//  ���ݸ��Ƕ��� 
/////////////////////////////////////////////////////////
#define SIS_DATA_ZIP     'R'   // ���濪ʼΪ���� ѹ����ʽ��ͷ��Ӧ�����ֶ�˵��
#define SIS_DATA_STRUCT  'B'   // ���濪ʼΪ���� �����ƽṹ������
#define SIS_DATA_STRING  'S'   // ���濪ʼΪ���� �ַ���
#define SIS_DATA_JSON    '{'   // ֱ�Ӵ�����  json�ĵ� ���������ʧ������ groups ��ʾ��֧��Ʊ������
							   // value ��ʾ���µ�һά����  values ��ʾ��ά����	
#define SIS_DATA_ARRAY   '['   // ֱ�Ӵ�����
#define SIS_DATA_CSV     'C'   // csv��ʽ����Ҫ�����ַ�������Ϣ

// #define SIS_INIT_WAIT    0 // 900��տ��� ��ʼ�ȴ���ʼ��
// #define SIS_INIT_WORK    1 // �յ�now��������һ��ֱ�Ӹ�״̬������������µģ��ͳ�ʼ�����״̬ 
// #define SIS_INIT_STOP    2 // ����

#define SIS_FIELD_METHOD_COVER 0 // ֱ���滻
#define SIS_FIELD_METHOD_MIN 1   // ��Сֵ���ų�0
#define SIS_FIELD_METHOD_MAX 2   // ���ֵ
#define SIS_FIELD_METHOD_INIT 3  // ���������Ϊ0���滻�������滻
#define SIS_FIELD_METHOD_INCR 4  // ������

/////////////////////////////////////////////////////////
//  �ֶ����Ͷ���
/////////////////////////////////////////////////////////
//����ʱ��Ķ���
#define SIS_FIELD_NONE 0 // "NONE"  //
// #define SIS_FIELD_TIME    7 // "TIME"  //int64 ����
// #define SIS_FIELD_INDEX   8 // "INDEX" //int32 һ��������ֵ
//8λ���붨��
// #define SIS_FIELD_CODE    9 // "CODE"  //int64 -- char[8] 8λ�ַ�ת��Ϊһ��int64Ϊ����
//�������Ͷ���
#define SIS_FIELD_STRING 10 // "STRING"  //string ���� �����������;
//�����ʽΪ field����:��������:����; SIS_FIELD_SIS_NG�����Ĭ��Ϊ16;
#define SIS_FIELD_INT 11	// "INT"    //int ����
#define SIS_FIELD_UINT 12   // "UINT"    //unsigned int ����
// #define SIS_FIELD_FLOAT 13  // "FLOAT"  //float
#define SIS_FIELD_DOUBLE 14 // "DOUBLE" //double & float


#pragma pack(push,1)
typedef struct s_sis_map_define{
	const char *key;
	uint8	style;  // ���
	uint16	uid;
	uint32  size;
}s_sis_map_define;

typedef struct s_sis_db {
	s_sis_sds name;  // ���ݿ�����
	s_sis_map_pointer  *db; // һ���ֵ��
	s_sis_map_pointer  *map; // �ؼ��ֲ�ѯ��

	s_sis_struct_list  *trade_time; // [[930,1130],[1300,1500]] ����ʱ�䣬����������

    // int    work_mode;
    // s_sis_struct_list  *work_plans;   // plans-work ��ʱ���� uint16 ������
    // s_sis_time_gap      work_always;  // always-work ѭ�����е�����

    int    save_mode;   // ���̷�ʽ�����ʱ��棬����ָ��ʱ���
    s_sis_struct_list  *save_plans;   // plans-work ��ʱ���� uint16 ������
    s_sis_time_gap      save_always;  // always-work ѭ�����е�����

	// int  save_type;  // ���̷�ʽ�����ʱ��棬����ָ��ʱ���
	// int  save_gaps;  // ���̵ļ������	
	// s_sis_struct_list  *save_plans; // uin16��ʱ�����У��������Ϊgap���ͱ�ʾ��Ϊ��λ�ļ��ʱ��
	s_sis_thread_id_t save_pid;
	s_sis_mutex_t save_mutex;  // saveʱ������

	s_sis_sds conf;    // ����ʱʹ��
	int save_format;   // �����ļ��ķ�ʽ

	s_sis_wait thread_wait; //�߳��ڲ���ʱ����

}s_sis_db;
#pragma pack(pop)


s_sis_db *sis_db_create(char *name);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sis_db_destroy(s_sis_db *);  //�ر�һ�����ݿ�

s_sis_sds sis_db_get_table_info_sds(s_sis_db *);


s_sis_map_define *sis_db_find_map_define(s_sis_db *, const char *name_, uint8 style_);
int sis_db_find_map_uid(s_sis_db *, const char *name_, uint8 style_);


#endif  /* _SIS_DB_H */
