
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_TABLE_H
#define _SISDB_TABLE_H

#include "sis_core.h"
// #include "dict.h"
// #include "sis_map.h"
#include "sis_list.h"
#include "sis_json.h"
// #include "sis_math.h"
#include "sis_time.h"

// #include "sisdb_fields.h"
#include "sisdb.h"


#pragma pack(push,1)

typedef struct s_sis_table_control {
	uint32 version;      // ���ݱ�İ汾��time_t��ʽ
	uint8  data_type;    // �������� Ŀǰûʲô��
	uint8  time_scale;   // ʱ��ѹ���Ĳ���
	uint32 limit_rows;   // ÿ��collection������¼��
	uint16 insert_mode;  // �������ݷ�ʽ
	// uint8  update_mode;  // �޸����ݷ�ʽ
	uint8  isinit;       // �Ƿ���Ҫ��ʼ���� ����ʱ�䵽��Ҫ���������
}s_sis_table_control;

// #define SIS_TABLE_LINK_COVER  0
// #define SIS_TABLE_LINK_INCR   1

typedef struct s_sis_table {
	s_sis_sds name;            //�������
	s_sis_db *father;            //���ݿ��ָ�룬��install���ʱ��ֵ
	s_sis_table_control control;       // ����ƶ���
	s_sis_string_list  *links;         // ���޸ı����ݱ�ʱ��ͬʱ��Ҫ�޸ĵ��������ݱ�
	s_sis_string_list  *field_name;      // ��˳���ŵ�����
	s_sis_map_pointer  *field_map;       // �ֶζ����ֵ�����ֶ����洢���ֶ��ڴ�飬ָ��sis_field
	s_sis_map_pointer  *collect_map;     // ���ݶ����ֵ������Ʊ���洢�������ڴ�飬ָ��sis_collect

	bool catch;   // �Ƿ��collect����catch��
	bool zip; 
	bool loading;  // �Ƿ���̼����У��ǵĻ��Ͳ����ַ�

}s_sis_table;

#pragma pack(pop)

s_sis_table *sis_table_create(s_sis_db *db_,const char *name_, s_sis_json_node *command);  //commandΪһ��json��ʽ�ֶζ���
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

void sis_table_destroy(s_sis_table *);  //ɾ��һ����
void sis_table_collect_clear(s_sis_table *);    //����һ�������������

//ȡ���ݺ�д����
s_sis_table *sisdb_get_table(s_sis_db *, const char *name_); //name -- table name

//�����ݿ�ĸ�����������
void sis_table_set_ver(s_sis_table *, uint32);  // time_t��ʽ
void sis_table_set_limit_rows(s_sis_table *, uint32); // 0 -- ������  1 -- ֻ�������µ�һ��  n 
void sis_table_set_insert_mode(s_sis_table *, uint16); // 1 -- �жϺ��޸� 0 2

void sis_table_set_fields(s_sis_table *, s_sis_json_node *fields_); //commandΪһ��json��ʽ�ֶζ���
//�õ���¼�ĳ���
int sis_table_get_fields_size(s_sis_table *);

// ��ȡʱ������,Ĭ��Ϊ��һ���ֶΣ�����һ���ֶβ����ϱ�׼��������

uint64 sis_table_struct_trans_time(uint64 in_, int inscale_, s_sis_table *out_tb_, int outscale_);

// һ������ͬʱд�����
int sis_table_update_and_pubs(int type_, s_sis_table *, const char *key_, const char *in_, size_t ilen_);
//  ֱ�Ӵӿ�����أ���Ҫ������У����أ��ӿ�����ٶ�
int sis_table_update_load(int type_, s_sis_table *table_, const char *key_, const char *in_, size_t ilen_);

// ��Դ������json����struct��table��struct����
// int sis_table_update(int type_, s_sis_table *, const char *key_, const char * in_, size_t ilen_);
//�޸����ݣ�key_Ϊ��Ʊ������г���ţ�value_Ϊ�����ƽṹ�����ݻ�json����
s_sis_sds sis_table_get_sds(s_sis_table *, const char *key_, const char *com_);  //����������Ҫ�ͷ�
s_sis_sds sis_table_get_code_sds(s_sis_table *, const char *key_, const char *com_);  //����������Ҫ�ͷ�
s_sis_sds sis_table_get_table_sds(s_sis_table *tb_, const char *com_);

s_sis_sds sis_table_get_collects_sds(s_sis_table *, const char *com_);  //����������Ҫ�ͷ�

s_sis_sds sis_table_get_search_sds(s_sis_table *tb_, const char *code_, int min_,int max_);

// commandΪjson����
//�����д���Ϊkey�����ݣ�keyΪ*��ʾ���й�Ʊ���ݣ���command�������ݷ�Χ���ֶη�Χ
//�û������command�йؼ��ֵĶ������£�
//�������ݸ�ʽ��"format":"json" --> SIS_DSIS_JSON
//						 "array" --> SIS_DATA_TYPE_ARRAY
//						 "bin" --> SIS_DATA_TYPE_STRUCT  ----> Ĭ��
//					     "string" --> SIS_DATA_TYPE_STRING
//						 "zip" --> SIS_DATA_TYPE_ZIP
//�ֶΣ�    "fields":  "time,close,vol,name" ��ʾһ��4���ֶ�  
//	
//                      ��,*---->��ʾȫ���ֶ�
//---------<��������û�б�ʾȫ������>--------
//���ݷ�Χ��"search":   min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//						forceΪ0��ʾ��ʵ��ȡ��Ϊ1�������ݾ�ȡminǰһ�����ݣ�
//���ݷ�Χ��"range":    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),

int sis_table_delete(s_sis_table *, const char *key_, const char *command);// commandΪjson����
//ɾ��
//�û������command�йؼ��ֵĶ������£�
//���ݷ�Χ��"search":   min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//���ݷ�Χ��"range":    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),


#endif  /* _SIS_TABLE_H */
