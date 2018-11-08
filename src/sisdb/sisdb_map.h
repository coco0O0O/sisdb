
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_MAP_H
#define _SISDB_MAP_H

#include "sis_core.h"
#include "sis_map.h"

#define SIS_MAP_DEFINE_TABLE_TYPE   0   // ������ -  û��
#define SIS_MAP_DEFINE_TIME_FORMAT  1   // ʱ���ʽ
#define SIS_MAP_DEFINE_FIELD_TYPE   2   // �ֶ�����
#define SIS_MAP_DEFINE_TIME_SCALE   3   // ʱ��߶�
#define SIS_MAP_DEFINE_ADD_METHOD   4   // ��β�������
#define SIS_MAP_DEFINE_ZIP_MODE     5   // ѹ������
#define SIS_MAP_DEFINE_SUBS_METHOD  6   // ��������д�뷽��
#define SIS_MAP_DEFINE_DATA_TYPE    7   // ��ȡд����������

// ����ת��ֻ�ܴӵ͵��ߣ����dayҪתminһ��ʧ��
#define SIS_TABLE_TYPE_STS     0 // struct time serial  Ĭ�ϸ�ʽ
#define SIS_TABLE_TYPE_JSON    1 // "json" 

// #define SIS_TABLE_TYPE_MARKET    100 //�г���
// #define SIS_TABLE_TYPE_STOCKS    101 //��Ʊ��

// ����ת��ֻ�ܴӵ͵��ߣ����dayҪתminһ��ʧ��
// #define SIS_TIME_FORMAT_NONE    0 // "NONE"  //
// #define SIS_TIME_FORMAT_INCR    1 // "INCR"  //int16 0��ʼ�ĵ���������Ӧ���з���
// #define SIS_TIME_FORMAT_MSEC    2 // "MSEC"  //int64 ��ʽ����ȷ������  
// #define SIS_TIME_FORMAT_SECOND  3 // "SECOND"  //int32 time_t��ʽ����ȷ����  
// #define SIS_TIME_FORMAT_DATE    4 // "DATE"  //int32 20170101��ʽ����ȷ����

/////////////////////////////////////////////////////////
//  �ֶ����Ͷ���
/////////////////////////////////////////////////////////
#define SIS_FIELD_TYPE_NONE    0  // "NONE"  //
#define SIS_FIELD_TYPE_INT     1  // "INT"    //int ����
#define SIS_FIELD_TYPE_UINT    2  // "UINT"    //unsigned int ����
#define SIS_FIELD_TYPE_FLOAT   3  // "FLOAT"  //float
#define SIS_FIELD_TYPE_CHAR    4  // "CHAR"  �����ַ���
#define SIS_FIELD_TYPE_STRING  5  // "STRING"  // �������ַ���;
//�����ʽΪ field����:��������:����; SIS_FIELD_TYPE_STRING�����Ĭ��Ϊ16;SIS
#define SIS_FIELD_TYPE_JSON    11  // "JSON"    // json��ʽ�ַ���;
#define SIS_FIELD_TYPE_PRICE   12  // "PRICE"   // רָ�۸���Ҫ�͹�Ʊinfo�е�С����ƥ��
#define SIS_FIELD_TYPE_VOLUME  13  // "VOLUME"  // רָ�ɽ�������������zint�洢��
// zint 1������λ��2λȡֵΪ 00 = 1 | 01 = 16 | 10 = 256 | 11 = 65536
// β�������ݣ������Ա�ʾΪ32���ڵ������ͽ�
#define SIS_FIELD_TYPE_AMOUNT  14  // "AMOUNT"  // רָ�ɽ�����������zint�洢��
// �����������Ҫ��Ϊ��ѹ��ʱ����
#define SIS_FIELD_TYPE_MSEC    15 // "MSEC"  //int64 ��ʽ����ȷ������  
#define SIS_FIELD_TYPE_SECOND  16 // "SECOND"  //int32 time_t��ʽ����ȷ����  
#define SIS_FIELD_TYPE_DATE    17 // "DATE"  //int32 20170101��ʽ����ȷ����


// ����ת��ֻ�ܴӵ͵��ߣ����dayҪתminһ��ʧ��
#define SIS_TIME_SCALE_NONE    0 // "NONE"  //
#define SIS_TIME_SCALE_MSEC    1 // "MSEC"  //int64 ��ʽ����ȷ������  
#define SIS_TIME_SCALE_SECOND  2 // "SECOND"  //int32 time_t��ʽ����ȷ����  
#define SIS_TIME_SCALE_INCR    3 // "INCR"  //int16 0��ʼ�ĵ���������Ӧ���з���
#define SIS_TIME_SCALE_MIN1    4 // "MIN1"  //int32 time_t��ʽ����ȷ��1����
#define SIS_TIME_SCALE_MIN5    5 // "MIN5"  //int32 time_t��ʽ ����ȷ��5����
#define SIS_TIME_SCALE_HOUR    6 // "HOUR"  //int32 time_t��ʽ����ȷ����Сʱ
#define SIS_TIME_SCALE_DATE    7 // "DATE"  //int32 20170101��ʽ����ȷ����
#define SIS_TIME_SCALE_WEEK    8 // "WEEK"  //int32 20170101��ʽ����ȷ����
#define SIS_TIME_SCALE_MONTH   9 // "MONTH"  //int32 20170101��ʽ����ȷ����
#define SIS_TIME_SCALE_YEAR   10 // "YEAR"  //int32 20170101��ʽ����ȷ����

/////////////////////////////////////////////////////////
//  ���ݿ����ݲ�����޸�ģʽ
/////////////////////////////////////////////////////////
#define SIS_ADD_METHOD_ALWAYS      0  // �����ж�ֱ��׷��
#define SIS_ADD_METHOD_TIME        1  // ���ʱ��ڵ��ظ��͸����ϵ����ݣ����ظ��ͷŶ�Ӧλ��
#define SIS_ADD_METHOD_VOL         2  // ���ɽ������ɽ������Ӿ�д��
#define SIS_ADD_METHOD_CODE        4  // �����ж�ֱ��׷��
#define SIS_ADD_METHOD_SORT        8  // ��ʱ������ͬһ��ʱ������ж������
#define SIS_ADD_METHOD_NONE        16  // ���ܲ���ֻ���޸� һ�㲻��

// #define SIS_INIT_WAIT    0 // 900��տ��� ��ʼ�ȴ���ʼ��
// #define SIS_INIT_WORK    1 // �յ�now��������һ��ֱ�Ӹ�״̬������������µģ��ͳ�ʼ�����״̬ 
// #define SIS_INIT_STOP    2 // ����

#define SIS_SUBS_METHOD_COPY 0 // ֱ���滻
#define SIS_SUBS_METHOD_MIN  1   // ��Сֵ���ų�0
#define SIS_SUBS_METHOD_MAX  2   // ���ֵ
#define SIS_SUBS_METHOD_ONCE 3  // ���������Ϊ0���滻�������滻
#define SIS_SUBS_METHOD_INCR 4  // ������

/////////////////////////////////////////////////////////
//  ���ݸ��Ƕ��� 
/////////////////////////////////////////////////////////
#define SIS_DATA_TYPE_ZIP     'Z'   // ���濪ʼΪ���� ѹ����ʽ��ͷ��Ӧ�����ֶ�˵��
#define SIS_DATA_TYPE_STRUCT  'B'   // ���濪ʼΪ���� �����ƽṹ������
#define SIS_DATA_TYPE_STRING  'S'   // ���濪ʼΪ���� �ַ���
#define SIS_DATA_TYPE_JSON    'J'   // ֱ�Ӵ�����  json�ĵ� ���������ʧ������ groups ��ʾ��֧��Ʊ������
							        // value ��ʾ���µ�һά����  values ��ʾ��ά����	
#define SIS_DATA_TYPE_ARRAY   'A'   // ֱ�Ӵ�����
#define SIS_DATA_TYPE_CSV     'C'   // csv��ʽ����Ҫ�����ַ�������Ϣ

#pragma pack(push,1)
 
typedef struct s_sis_map_define{
	const char *key;
	uint8	style;  // ���
	uint16	uid;
	uint32  size;
} s_sis_map_define;

#pragma pack(pop)

void sisdb_init_map_define(s_sis_map_pointer *fields_);

s_sis_map_define *sisdb_find_map_define(s_sis_map_define *, const char *name_, uint8 style_);
int sisdb_find_map_uid(s_sis_map_define *, const char *name_, uint8 style_);


#endif  /* _SISDB_H */
