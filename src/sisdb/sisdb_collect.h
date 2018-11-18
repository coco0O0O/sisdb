
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_COLLECT_H
#define _SISDB_COLLECT_H

#include "sis_core.h"
#include "sis_math.h"
#include "sis_malloc.h"

#include "sisdb.h"
#include "sisdb_sys.h"
#include "sisdb_table.h"

/////////////////////////////////////////////////////////
//  ���ݿ���������ģʽ
/////////////////////////////////////////////////////////
#define SIS_SEARCH_NONE -1 // û�����ݷ�������
#define SIS_SEARCH_NEAR  1  // ����������
#define SIS_SEARCH_LEFT  2  // ��Ȼ���������ݣ������ص����ݱ������ѯ�ĸ���һЩ
#define SIS_SEARCH_RIGHT 3  // ��Ȼ���������ݣ������ص����ݱ������ѯ�ĸ���һЩ
#define SIS_SEARCH_OK    0	// ׼ȷƥ������ݣ�ʱ��һ��

#define SIS_CHECK_LASTTIME_INIT  0   // �������¼�¼���ȼ򵥰��������ж�
#define SIS_CHECK_LASTTIME_NEW   1   // ����������¼��������ʱ��������һ����¼
#define SIS_CHECK_LASTTIME_OLD   2   // ��¼��������ʱ��С�����һ����¼
#define SIS_CHECK_LASTTIME_OK    3   // �������һ����¼
#define SIS_CHECK_LASTTIME_ERROR 4   // ���󣬲�����

#define SIS_JSON_KEY_ARRAY ("value")   // ��ȡһ����Ʊһ������
#define SIS_JSON_KEY_ARRAYS ("values") //��ȡһ����Ʊ�������
#define SIS_JSON_KEY_GROUPS ("groups")  //��ȡ�����Ʊ����
#define SIS_JSON_KEY_FIELDS ("fields")  // �ֶζ���
#define SIS_JSON_KEY_COLLECTS ("collects") // ��ȡһ��db�����й�Ʊ����

// #define SIS_UPDATE_MODE_NO     0   // ��������
// #define SIS_UPDATE_MODE_UPDATE   1   // �޸�ĳ����¼
// #define SIS_UPDATE_MODE_INSERT   2   // ������ĳ����¼֮ǰ
// #define SIS_UPDATE_MODE_OK    1   // ����������

#pragma pack(push, 1)
// ���ݽṹ�������ʱ�����У��Զ�����ͷβʱ�䣬��ƽ�����ʱ��
typedef struct s_sis_step_index
{
	uint32 count;
	uint64 left; // ��Сʱ��
	uint64 right;
	double step; // ���ʱ�䣬ÿ����¼��Լ���ʱ�䣬
} s_sis_step_index;

// ������Ʊ�����ݰ�
// ʵ���϶�����һ��������ⲿ����ֱ��ͨ��SH600600.DAY���ʵ�collect��
// ��collect�ṹ�͹����ֱ���Ϊ�����ᣬ��Ψһȷ��������������ȱһ���ɣ�
// �������������Ŀ���ǿ��ٶ�λĿ�����ݣ�ͬʱ�����������ݣ�
// 
typedef struct s_sisdb_collect
{
	s_sisdb_table     *db;    // ���ָ�룬���Ի���ֶζ���������Ϣ

	////////////////////////////////////////////////////////////
	//   ���������ṹ������ר�����ݿ�ʱʹ��
	s_sisdb_sys_exch  *spec_exch; // �г�����Ϣ
	s_sisdb_sys_info  *spec_info; // ��Ʊ����Ϣ
	////////////////////////////////////////////////////////////
	s_sis_step_index  *stepinfo; // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	s_sis_struct_list *value;    // �ṹ������

	s_sis_sds front;  // ǰһ���ӵļ�¼ catch=true��Ч volΪȫ�� -- ����ʱһ��Ҫ����
	s_sis_sds lasted; // ��ǰ��һ���ӵļ�¼ catch=true��Ч volΪȫ�� -- ����ʱһ��Ҫ����

	s_sis_sds refer;  // ʵ�����ݵ�ǰһ���ο����� zip ʱ��Ч volΪ���� -- ��Ҫ����
} s_sisdb_collect;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_step_index --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sis_step_index *sisdb_stepindex_create();
void sisdb_stepindex_destroy(s_sis_step_index *);
void sisdb_stepindex_clear(s_sis_step_index *);
void sisdb_stepindex_rebuild(s_sis_step_index *, uint64 left_, uint64 right_, int count_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_sisdb_collect --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sisdb_collect *sisdb_collect_create(s_sis_db *db_, const char *key_);
void sisdb_collect_destroy(s_sisdb_collect *);
void sisdb_collect_clear(s_sisdb_collect *unit_);

s_sisdb_collect *sisdb_get_collect(s_sis_db *db_, const char *key_);

int sisdb_collect_recs(s_sisdb_collect *unit_);

int sisdb_collect_search(s_sisdb_collect *unit_, uint64 index_);
int sisdb_collect_search_left(s_sisdb_collect *unit_, uint64 index_, int *mode_);
int sisdb_collect_search_right(s_sisdb_collect *unit_, uint64 index_, int *mode_);

s_sis_sds sisdb_collect_get_of_range_sds(s_sisdb_collect *, int start_, int stop_);
s_sis_sds sisdb_collect_get_of_count_sds(s_sisdb_collect *, int start_, int count_);

///////////////////////////
//			get     ///////
///////////////////////////
//�������ʱ���Ѷ����ƽṹ����ת����json��ʽ���ݣ�����array�����ݣ�json ����Ҫ���fields�ṹ
s_sis_sds sisdb_collect_struct_filter_sds(s_sisdb_collect *unit_, s_sis_sds in_, s_sis_string_list *fields_);
s_sis_sds sisdb_collect_struct_to_json_sds(s_sisdb_collect *unit_, s_sis_sds in_, s_sis_string_list *fields_, bool zip_);
s_sis_sds sisdb_collect_struct_to_array_sds(s_sisdb_collect *unit_, s_sis_sds in_, s_sis_string_list *fields_, bool zip_);

s_sis_sds sisdb_collect_struct_to_csv_sds(s_sisdb_collect *unit_, s_sis_sds in_, s_sis_string_list *fields_, bool zip_);

// �õ�������ԭʼ���ݣ�
s_sis_sds sisdb_collect_get_original_sds(s_sisdb_collect *collect, s_sis_json_handle *handle);
// �õ������������
s_sis_sds sisdb_collect_get_sds(s_sis_db *db_,const char *, const char *com_);
// commandΪjson����
// �����д���Ϊkey�����ݣ�keyΪ*��ʾ���й�Ʊ���ݣ���command�������ݷ�Χ���ֶη�Χ
// �û������command�йؼ��ֵĶ������£�
// �������ݸ�ʽ��"format":"json" --> SIS_DSIS_JSON
//						 "array" --> SIS_DATA_TYPE_ARRAY
//						 "bin" --> SIS_DATA_TYPE_STRUCT  ----> Ĭ��
//					     "string" --> SIS_DATA_TYPE_STRING
//						 "zip" --> SIS_DATA_TYPE_ZIP
// �ֶΣ�    "fields":  "time,close,vol,name" ��ʾһ��4���ֶ�  
//	
//                      ��,*---->��ʾȫ���ֶ�
// ---------<��������û�б�ʾȫ������>--------
// ���ݷ�Χ��"search":  min �� start ���⣬min��ʾ����ֵȡ����start��ʾ����¼��ȡ 
// 					min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//						forceΪ0��ʾ��ʵ��ȡ��Ϊ1�������ݾ�ȡminǰһ�����ݣ�
//				    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),
// �õ����й�Ʊ���µ�һ����¼
s_sis_json_node *sisdb_collect_groups_json_init(s_sis_string_list *fields_);
void sisdb_collect_groups_json_push(s_sis_json_node *node_, char *code, s_sisdb_collect *unit_, s_sis_sds in_, s_sis_string_list *fields_);
s_sis_sds sisdb_collect_groups_json_sds(s_sis_json_node *node_);
//////////////////////
// �õ������Ʊ�����һ�����ݼ���
s_sis_sds sisdb_collects_get_last_sds(s_sis_db *db_,const char *, const char *com_);

///////////////////////////
//			delete     ////
///////////////////////////
int sisdb_collect_delete_of_range(s_sisdb_collect *, int start_, int stop_);  // ��λ��ɾ��
int sisdb_collect_delete_of_count(s_sisdb_collect *, int start_, int count_); // ��λ��ɾ��

int sisdb_collect_delete(s_sis_db  *, const char *key_, const char *com_); // commandΪjson����
//ɾ��
//�û������command�йؼ��ֵĶ������£�
//���ݷ�Χ��"search":   min �� start ���⣬min��ʾ����ֵȡ����start��ʾ����¼��ȡ 
// 					  min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//                    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),

///////////////////////////
//			set        ////
///////////////////////////
//����json����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sisdb_collect_json_to_struct_sds(s_sisdb_collect *, const char *in_, size_t ilen_);

//����array����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sisdb_collect_array_to_struct_sds(s_sisdb_collect *, const char *in_, size_t ilen_);

int sisdb_collect_update(s_sisdb_collect *unit_, s_sis_sds in_);

int sisdb_collect_update_publish(s_sisdb_collect *unit_,s_sis_sds val_, const char *code_);

// �Ӵ��̼��أ�����д�룬
int sisdb_collect_update_block(s_sisdb_collect *, const char *in_, size_t ilen_);

#endif /* _SIS_COLLECT_H */
