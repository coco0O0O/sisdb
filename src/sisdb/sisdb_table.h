
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_TABLE_H
#define _SISDB_TABLE_H

#include "sis_core.h"
#include "sis_list.h"
#include "sis_json.h"
#include "sis_time.h"
#include "sis_method.h"

#include "sisdb.h"

#pragma pack(push,1)

#define SIS_WRITE_ALWAYS           0  // �����ж�ֱ��׷��
#define SIS_WRITE_CHECKED          1  // ��Ҫ���
#define SIS_WRITE_SORT_TIME        2  // ��ʱ������
#define SIS_WRITE_SORT_OTHER       4  // �������ֶ�����
#define SIS_WRITE_SORT_NONE        8  // ������

#define SIS_WRITE_SOLE_TIME        16 // ֻ��ʱ��Ψһ
#define SIS_WRITE_SOLE_MULS        32 // ����ʱ���ֶλ��������ֶ�ͬʱΨһ 
#define SIS_WRITE_SOLE_OTHER       64 // û��ʱ���ֶΣ�һ�������ֶ�ͬʱΨһ 
#define SIS_WRITE_SOLE_NONE        128  // ���ж��Ƿ�Ψһ


typedef struct s_sisdb_table_control {
	uint8  type;         // ���ݱ����� Ŀǰûʲô��
	uint8  scale;        // ʱ��ѹ���Ĳ���
	uint32 limits;       // ÿ��collection������¼��

	uint8  issys;        // �߱�system�����ݱ��߱����ȴ洢�����ԣ����Ҳ�ʹ��infoָ��
	uint8  isinit;       // �Ƿ���Ҫ��ʼ���� ����ʱ�䵽��Ҫ���������
	uint8  issubs;       // �Ƿ��collect���� sub ���� catch��
	uint8  iszip; 		 // ���ݱ��Ƿ�ѹ���洢
}s_sisdb_table_control;

typedef struct s_sisdb_table {
	uint32    version;      		     // ���ݱ�İ汾��time_t��ʽ
	s_sis_sds name;                      // �������
	s_sis_db *father;                    // ���ݿ��ָ�룬��install���ʱ��ֵ

	uint32              write_style;     // д������
	s_sis_sds           write_sort;  	 // �����ֶ�
	s_sis_pointer_list *write_solely;    // Ψһ���ֶμ��� �ֶ�����
	s_sis_method_class *write_method;    // ���ݵļ�鷽��

	s_sisdb_table_control control;       // ����ƶ���
	s_sis_string_list  *publishs;        // ���޸ı����ݱ�ʱ��ͬʱ��Ҫ�޸ĵ��������ݱ�
	s_sis_string_list  *field_name;      // ��˳���ŵ�����
	s_sis_map_pointer  *field_map;       // �ֶζ����ֵ�����ֶ����洢���ֶ��ڴ�飬ָ�� s_sisdb_field
	s_sis_string_list  *collect_list;    // ������ issys Ϊ��ʱ����collect��key�ڴ���ʱ�´�push
}s_sisdb_table;

#pragma pack(pop)

s_sisdb_table *sisdb_table_create(s_sis_db *db_,const char *name_, s_sis_json_node *command);  //commandΪһ��json��ʽ�ֶζ���
void sisdb_table_destroy(s_sisdb_table *);  //ɾ��һ����

s_sisdb_table *sisdb_get_table(s_sis_db *db_, const char *dbname_);
s_sisdb_table *sisdb_get_table_from_key(s_sis_db *db_, const char *key_);

int sisdb_table_set_fields(s_sis_db *db_,s_sisdb_table *, s_sis_json_node *fields_); //commandΪһ��json��ʽ�ֶζ���
// ���ؼ�¼�ĳ���
int sisdb_table_get_fields_size(s_sisdb_table *);
// 

int sisdb_table_update(s_sis_db *db_,s_sisdb_table *, const char *key_ ,const char *val_ ,size_t len_); 

int sisdb_table_set_conf(s_sis_db *db_, const char *table_, s_sis_json_node *);

s_sis_json_node *sisdb_table_new_config(const char *val_ ,size_t len_);

#endif  /* _SIS_TABLE_H */
