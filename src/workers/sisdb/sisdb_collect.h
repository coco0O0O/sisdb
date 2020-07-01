
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_COLLECT_H
#define _SISDB_COLLECT_H

#include "sis_core.h"
#include "sis_math.h"
#include "sis_malloc.h"

#include "sisdb.h"
#include "sis_json.h"
#include "sis_dynamic.h"

/////////////////////////////////////////////////////////
//  ���ݿ���������ģʽ
/////////////////////////////////////////////////////////
#define SIS_SEARCH_NONE -1  // û�����ݷ�������
#define SIS_SEARCH_OK    0	// ׼ȷƥ������ݣ�ʱ��һ��
#define SIS_SEARCH_LEFT  1  // ��Ȼ���������ݣ������ص����ݱ������ѯ�ĸ���һЩ
#define SIS_SEARCH_RIGHT 2  // ��Ȼ���������ݣ������ص����ݱ������ѯ�ĸ���һЩ

#define SIS_CHECK_LASTTIME_NEW   1   // ����������¼��������ʱ��������һ����¼
#define SIS_CHECK_LASTTIME_OLD   2   // ��¼��������ʱ��С�����һ����¼
#define SIS_CHECK_LASTTIME_OK    3   // �������һ����¼
#define SIS_CHECK_LASTTIME_ERROR 4   // ���󣬲�����

#define SIS_SEARCH_MIN     0x01
#define SIS_SEARCH_MAX     0x02
#define SIS_SEARCH_START   0x04
#define SIS_SEARCH_COUNT   0x08
#define SIS_SEARCH_OFFSET  0x10
// #define SIS_UPDATE_MODE_NO     0   // ��������
// #define SIS_UPDATE_MODE_UPDATE   1   // �޸�ĳ����¼
// #define SIS_UPDATE_MODE_INSERT   2   // ������ĳ����¼֮ǰ
// #define SIS_UPDATE_MODE_OK    1   // ����������

#pragma pack(push,1)
// ���ݽṹ�������ʱ�����У��Զ�����ͷβʱ�䣬��ƽ�����ʱ��
typedef struct s_sis_step_index
{
	uint64 count;
	uint64 left; // ��Сʱ��
	uint64 right;
	double step; // ���ʱ�䣬ÿ����¼��Լ���ʱ�䣬
} s_sis_step_index;

// ������Ʊ�����ݰ�
// ʵ���϶�����һ��������ⲿ����ֱ��ͨ��SH600600.DAY���ʵ�collect��
// ��collect�ṹ�͹����ֱ���Ϊ�����ᣬ��Ψһȷ��������������ȱһ���ɣ�
// �������������Ŀ���ǿ��ٶ�λĿ�����ݣ�ͬʱ�����������ݣ�
// 

#define SISDB_COLLECT_TYPE_TABLE   0   // s_sis_struct_list
#define SISDB_COLLECT_TYPE_CHARS   1   // s_sis_sds
#define SISDB_COLLECT_TYPE_BYTES   2   // s_sis_sds
#define SISDB_COLLECT_TYPE_LIST    3   // s_sis_list s_sis_sds

// ����TICK���͵����ݱ� �洢��ʽ��ͬ ��һ��nodelist ��Ԫ���� ���+ʱ������+�����б�+�����б� s_struct_list
// ��ȡ����ʱ���ݲ�ͬ��� �����������б� ����������� ��Ҫ��������һ���߳� 
// ��ʷ����ֱ�Ӵ� sno �л�ȡ

typedef struct s_sisdb_collect
{
	uint8                style; 
	s_sis_object        *obj;  

	s_sis_sds            key;

	s_sisdb_cxt         *father;
	////////////////////////////////////////////////////////////
	s_sisdb_table       *sdb;    // sdb��ָ�룬���Ի���ֶζ���������Ϣ

	////////////////////////////////////////////////////////////
	s_sis_step_index    *stepinfo; // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	// s_sis_struct_list   *value;    // �ṹ������

} s_sisdb_collect;

typedef struct s_sisdb_collect_sno
{
	s_sisdb_collect   *collect;
	uint32             recno;    // ��¼��
} s_sisdb_collect_sno;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_step_index --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sis_step_index *sisdb_stepindex_create();
void sisdb_stepindex_destroy(s_sis_step_index *);
void sisdb_stepindex_clear(s_sis_step_index *);
void sisdb_stepindex_rebuild(s_sis_step_index *, uint64 left_, uint64 right_, int count_);
int sisdb_stepindex_goto(s_sis_step_index *si_, uint64 curr_);


///////////////////////////////////////////////////////////////////////////
//------------------------s_sisdb_collect --------------------------------//
///////////////////////////////////////////////////////////////////////////
s_sisdb_collect *sisdb_get_collect(s_sisdb_cxt *sisdb_, const char *key_);
s_sisdb_table *sisdb_get_table(s_sisdb_cxt *sisdb_, const char *dbname_);

s_sisdb_collect *sisdb_kv_create(uint8 style_, char *in_, size_t ilen_);
void sisdb_kv_destroy(void *info_);

s_sisdb_collect *sisdb_collect_create(s_sisdb_cxt *sisdb_,const char *key_);
void sisdb_collect_destroy(void *);
void sisdb_collect_clear(s_sisdb_collect *);

msec_t sisdb_collect_get_time(s_sisdb_collect *, int index_);
uint64 sisdb_collect_get_mindex(s_sisdb_collect *, int index_);

int sisdb_collect_recs(s_sisdb_collect *);

///////////////////////////////////////////////////////////////////////////
//  get
///////////////////////////////////////////////////////////////////////////

bool sisdb_collect_trans_of_range(s_sisdb_collect *collect_, int *start_, int *stop_);
bool sisdb_collect_trans_of_count(s_sisdb_collect *collect_, int *start_, int *stop_);

int sisdb_collect_search(s_sisdb_collect *, uint64 finder_);
int sisdb_collect_search_left(s_sisdb_collect *, uint64 finder_, int *mode_);
int sisdb_collect_search_right(s_sisdb_collect *, uint64 finder_, int *mode_);
int sisdb_collect_search_last(s_sisdb_collect *, uint64 finder_, int *mode_);

s_sis_sds sisdb_collect_get_of_range_sds(s_sisdb_collect *, int start_, int stop_);
s_sis_sds sisdb_collect_get_of_count_sds(s_sisdb_collect *, int start_, int count_);
s_sis_sds sisdb_collect_get_last_sds(s_sisdb_collect *);

// �õ�������ԭʼ���ݣ�
s_sis_sds sisdb_collect_get_original_sds(s_sisdb_collect *collect_, s_sis_json_node *);

s_sis_sds sisdb_collect_fastget_sds(s_sisdb_collect *collect_,const char *);
// �õ������������
s_sis_sds sisdb_collect_get_sds(s_sisdb_collect *collect_, const char *key_, int iformat_, s_sis_json_node *);
// �û������ argv �����Ĺؼ��ֵĶ������£�
// ���ݸ�ʽ��"format":"json" --> SIS_DSIS_JSON
//						"array" --> SIS_DATA_TYPE_ARRAY
//						"csv" --> SIS_DATA_TYPE_ARRAY
//						"bytes" --> SIS_DATA_TYPE_STRUCT  ----> Ĭ��
// �ֶΣ�   "fields":  "time,close,vol,name" ��ʾһ��4���ֶ�  
//	
//                      ��,*---->��ʾȫ���ֶ� Ĭ��ȫ���ֶ�
// ---------<��������û�б�ʾȫ������>--------
// ���ݷ�Χ��"search":  min �� start ���⣬min��ʾ����ֵȡ����start ��ʾ����¼��ȡ 
// 					  min,max ��ʱ������ȡ����
//						count(��max���⣬����ʾ��󣬸���ʾ��ǰ),
//						forceΪ 0 ��ʾ��ʵ��ȡ��Ϊ 1 �������ݾ�ȡminǰһ�����ݣ�
//				    start��stop ����¼��ȡ���� 0��-1-->��ʾȫ������
//						count(��stop���⣬����ʾ��󣬸���ʾ��ǰ),
// �õ����й�Ʊ���µ�һ����¼

// ȡ�����Ʊ��ϢĬ��ֻ�������һ�����ݼ���
s_sis_json_node *sisdb_collects_get_last_node(s_sisdb_collect *, s_sis_json_node *);

///////////////////////////
//	fromat trans
///////////////////////////
//�������ʱ���Ѷ����ƽṹ����ת����json��ʽ���ݣ�����array�����ݣ�json ����Ҫ���fields�ṹ
s_sis_json_node *sis_collect_get_fields_of_json(s_sisdb_collect *collect_, s_sis_string_list *fields_);
s_sis_sds sis_collect_get_fields_of_csv(s_sisdb_collect *collect_, s_sis_string_list *fields_);


s_sis_sds sisdb_collect_struct_to_sds(s_sisdb_collect *, s_sis_sds in_, s_sis_string_list *fields_);

s_sis_sds sisdb_collect_struct_to_json_sds(s_sisdb_collect *collect_, s_sis_sds in_,
										   const char *key_, s_sis_string_list *fields_, bool isfields_, bool zip_);

s_sis_sds sisdb_collect_struct_to_array_sds(s_sisdb_collect *, s_sis_sds in_, s_sis_string_list *fields_, bool zip_);

s_sis_sds sisdb_collect_struct_to_csv_sds(s_sisdb_collect *, s_sis_sds in_, s_sis_string_list *fields_, bool isfields_);
//����json����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sisdb_collect_json_to_struct_sds(s_sisdb_collect *, s_sis_sds in_);

//����array����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sisdb_collect_array_to_struct_sds(s_sisdb_collect *, s_sis_sds in_);

///////////////////////////
//	delete     ////
///////////////////////////
int sisdb_collect_delete_of_range(s_sisdb_collect *, int start_, int stop_);  // ��λ��ɾ��
int sisdb_collect_delete_of_count(s_sisdb_collect *, int start_, int count_); // ��λ��ɾ��

int sisdb_collect_delete(s_sisdb_collect  *, s_sis_json_node *jsql); // jsql Ϊjson����
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
// дͨ��sdb���� ��Ҫ�������ݺϷ���
int sisdb_collect_update(s_sisdb_collect *, s_sis_sds in_);

// дsno���� ��ʹ���ݻḲ�� ���ָ�ʽҲҪ��¼��ÿһ�α仯
int sisdb_collect_wseries(s_sisdb_collect *, s_sis_sds in_);

// �Ӵ���������д�룬����������У�� ֱ��׷������
int sisdb_collect_wpush(s_sisdb_collect *, char *in_, size_t ilen_);

#endif /* _SIS_COLLECT_H */
