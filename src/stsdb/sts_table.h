
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_TABLE_H
#define _STS_TABLE_H

#include "sts_core.h"
// #include "dict.h"
// #include "sts_map.h"
#include "sts_list.h"
#include "sts_json.h"
// #include "sts_math.h"
#include "sts_time.h"

#include "sts_fields.h"

/////////////////////////////////////////////////////////
//  ���ݿ����ݲ�����޸�ģʽ
/////////////////////////////////////////////////////////
#define STS_OPTION_ALWAYS      0  // �����ж�ֱ��׷��
#define STS_OPTION_TIME        1  // ���ʱ��ڵ��ظ��͸����ϵ����ݣ����ظ��ͷŶ�Ӧλ��
#define STS_OPTION_VOL         2  // ���ɽ������ɽ������Ӿ�д��
#define STS_OPTION_NONE        3  // �����ж�ֱ��׷��
#define STS_OPTION_MUL_CHECK   4  // ����3���������ݲ���ȷ�����ݵ�׼ȷ�ԣ��ݲ���

/////////////////////////////////////////////////////////
//  ���ݸ��Ƕ��� 
/////////////////////////////////////////////////////////
#define STS_DATA_ZIP     'R'   // ���濪ʼΪ���� ѹ����ʽ��ͷ��Ӧ�����ֶ�˵��
#define STS_DATA_STRUCT  'B'   // ���濪ʼΪ���� �����ƽṹ������
#define STS_DATA_STRING  'S'   // ���濪ʼΪ���� �ַ���
#define STS_DATA_JSON    '{'   // ֱ�Ӵ�����  json�ĵ� ���������ʧ������ groups ��ʾ��֧��Ʊ������
							   // value ��ʾ���µ�һά����  values ��ʾ��ά����	
#define STS_DATA_ARRAY   '['   // ֱ�Ӵ�����

#pragma pack(push,1)

typedef struct s_sts_db s_sts_db;

typedef struct s_sts_table_control {
	uint32 version;      // ���ݱ�İ汾��time_t��ʽ
	uint8  data_type;    // �������� Ŀǰûʲô��
	uint8  time_scale;   // ʱ��ѹ���Ĳ���
	uint32 limit_rows;   // ÿ��collection������¼��
	uint8  insert_mode;  // �������ݷ�ʽ
	uint8  update_mode;  // �޸����ݷ�ʽ
}s_sts_table_control;

#define STS_TABLE_LINK_COVER  0
#define STS_TABLE_LINK_INCR   1

typedef struct s_sts_table {
	sds name;            //�������
	s_sts_db *father;            //���ݿ��ָ�룬��install���ʱ��ֵ
	s_sts_table_control control;       // ����ƶ���
	s_sts_string_list  *links;         // ���޸ı����ݱ�ʱ��ͬʱ��Ҫ�޸ĵ��������ݱ�
	s_sts_string_list  *field_name;      // ��˳���ŵ�����
	s_sts_map_pointer  *field_map;       // �ֶζ����ֵ�����ֶ����洢���ֶ��ڴ�飬ָ��sts_field_unit
	s_sts_map_pointer  *collect_map;     // ���ݶ����ֵ������Ʊ���洢�������ڴ�飬ָ��sts_collect_unit

	bool catch;   // �Ƿ��collect����catch��

}s_sts_table;

#pragma pack(pop)

s_sts_table *sts_table_create(s_sts_db *db_,const char *name_, s_sts_json_node *command);  //commandΪһ��json��ʽ�ֶζ���
// commandΪjson����
//�û������command�йؼ��ֵĶ������£�
//�ֶζ��壺  "fields":  []
//  # �ֶ���| ��������| ����| io �Ŵ�����С| ���ű��� zoom|ѹ������|ѹ���ο��ֶ�����
//  [name, string, 16, 0, 0, 0, 0],

//��¼�����ƣ�"limit":  0 ��¼������  
//ʱ�����еĳ߶�  scale
//�������ݷ�ʽ��  "insert":  push �����ж�ֱ������ incr-time ����ʱ���������Ӽ�¼�������Ӿ�ˢ���ϼ�¼
	//  incr-vol ���ݳɽ����������������ݣ�
	//  ���limitΪ1���������޸ĵ�һ����¼

void sts_table_destroy(s_sts_table *);  //ɾ��һ����
void sts_table_clear(s_sts_table *);    //����һ�������������
//�����ݿ�ĸ�����������
void sts_table_set_ver(s_sts_table *, uint32);  // time_t��ʽ
void sts_table_set_limit_rows(s_sts_table *, uint32); // 0 -- ������  1 -- ֻ�������µ�һ��  n 
void sts_table_set_insert_mode(s_sts_table *, uint8_t); // 1 -- �жϺ��޸� 0 2

void sts_table_set_fields(s_sts_table *, s_sts_json_node *fields_); //commandΪһ��json��ʽ�ֶζ���
//�õ���¼�ĳ���
int sts_table_get_fields_size(s_sts_table *);

// ��ȡʱ������,Ĭ��Ϊ��һ���ֶΣ�����һ���ֶβ����ϱ�׼��������

uint64 sts_table_struct_trans_time(uint64 in_, int inscale_, s_sts_table *out_tb_, int outscale_);

// һ������ͬʱд�����
int sts_table_update_mul(int type_, s_sts_table *, const char *key_, const char *in_, size_t ilen_);

// ��Դ������json����struct��table��struct����
int sts_table_update(int type_, s_sts_table *, const char *key_, const char * in_, size_t ilen_);
//�޸����ݣ�key_Ϊ��Ʊ������г���ţ�value_Ϊ�����ƽṹ�����ݻ�json����
sds sts_table_get_m(s_sts_table *, const char *key_, const char *command);  //����������Ҫ�ͷ�

// commandΪjson����
//�����д���Ϊkey�����ݣ�keyΪ*��ʾ���й�Ʊ���ݣ���command�������ݷ�Χ���ֶη�Χ
//�û������command�йؼ��ֵĶ������£�
//�������ݸ�ʽ��"format":"json" --> STS_DATA_JSON
//						 "array" --> STS_DATA_ARRAY
//						 "bin" --> STS_DATA_STRUCT  ----> Ĭ��
//					     "string" --> STS_DATA_STRING
//						 "zip" --> STS_DATA_ZIP
//�ֶΣ�    "fields":  "time,close,vol,name" ��ʾһ��4���ֶ�  
//	
//                      ��,*---->��ʾȫ���ֶ�
//---------<��������û�б�ʾȫ������>--------
//���ݷ�Χ��"search":   min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//						forceΪ0��ʾ��ʵ��ȡ��Ϊ1�������ݾ�ȡminǰһ�����ݣ�
//���ݷ�Χ��"range":    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),

int sts_table_delete(s_sts_table *, const char *key_, const char *command);// commandΪjson����
//ɾ��
//�û������command�йؼ��ֵĶ������£�
//���ݷ�Χ��"search":   min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//���ݷ�Χ��"range":    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),

#endif  /* _STS_TABLE_H */
