
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_TABLE_H
#define _SISDB_TABLE_H

#include "sis_core.h"
#include "sis_list.h"
#include "sis_json.h"
#include "sis_time.h"

#include "sisdb.h"

#pragma pack(push,1)

typedef struct s_sisdb_table_control {
	uint8  type;         // ���ݱ����� Ŀǰûʲô��
	uint8  scale;        // ʱ��ѹ���Ĳ���
	uint32 limits;       // ÿ��collection������¼��

	uint8  isinit;       // �Ƿ���Ҫ��ʼ���� ����ʱ�䵽��Ҫ���������
	uint8  issubs;       // �Ƿ��collect���� sub ���� catch��
	uint8  iszip; 		 // ���ݱ��Ƿ�ѹ���洢
	uint8  isloading;    // �Ƿ���̼����У��ǵĻ��Ͳ����ַ�
}s_sisdb_table_control;

// #define SIS_TABLE_LINK_COVER  0
// #define SIS_TABLE_LINK_INCR   1

typedef struct s_sisdb_table {
	uint32    version;      		     // ���ݱ�İ汾��time_t��ʽ
	s_sis_sds name;                      // �������
	s_sis_db *father;                    // ���ݿ��ָ�룬��install���ʱ��ֵ
	uint16    append_method;             // �������ݷ�ʽ
	s_sisdb_table_control control;       // ����ƶ���
	s_sis_string_list  *publishs;        // ���޸ı����ݱ�ʱ��ͬʱ��Ҫ�޸ĵ��������ݱ�
	s_sis_string_list  *field_name;      // ��˳���ŵ�����
	s_sis_map_pointer  *field_map;       // �ֶζ����ֵ�����ֶ����洢���ֶ��ڴ�飬ָ�� s_sisdb_field

}s_sisdb_table;

#pragma pack(pop)

s_sisdb_table *sisdb_table_create(s_sis_db *db_,const char *name_, s_sis_json_node *command);  //commandΪһ��json��ʽ�ֶζ���
void sisdb_table_destroy(s_sisdb_table *);  //ɾ��һ����

int sisdb_table_set_fields(s_sisdb_table *, s_sis_json_node *fields_); //commandΪһ��json��ʽ�ֶζ���
// ���ؼ�¼�ĳ���
int sisdb_table_get_fields_size(s_sisdb_table *);
// 

// int sisdb_table_update_publish(s_sisdb_table *table_,const char *key_, 
// 	s_sisdb_collect *collect_,s_sis_sds val_)

// ��ȡʱ������,Ĭ��Ϊ��һ���ֶΣ�����һ���ֶβ����ϱ�׼��������
// uint64 sisdb_table_struct_trans_time(uint64 in_, int inscale_, s_sisdb_table *out_tb_, int outscale_);

// һ������ͬʱд�����
// int sisdb_table_update_and_pubs(int type_, s_sisdb_table *, const char *key_, const char *in_, size_t ilen_);
// //  ֱ�Ӵӿ�����أ���Ҫ������У����أ��ӿ�����ٶ�
// int sisdb_table_update_load(int type_, s_sisdb_table *table_, const char *key_, const char *in_, size_t ilen_);

// // ��Դ������json����struct��table��struct����
// // int sisdb_table_update(int type_, s_sisdb_table *, const char *key_, const char * in_, size_t ilen_);
// //�޸����ݣ�key_Ϊ��Ʊ������г���ţ�value_Ϊ�����ƽṹ�����ݻ�json����
// s_sis_sds sisdb_table_get_sds(s_sisdb_table *, const char *key_, const char *com_);  //����������Ҫ�ͷ�
// s_sis_sds sisdb_table_get_code_sds(s_sisdb_table *, const char *key_, const char *com_);  //����������Ҫ�ͷ�

// s_sis_sds sisdb_table_get_search_sds(s_sisdb_table *tb_, const char *code_, int min_,int max_);


#endif  /* _SIS_TABLE_H */
