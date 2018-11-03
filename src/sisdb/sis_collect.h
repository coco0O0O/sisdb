
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SIS_COLLECT_H
#define _SIS_COLLECT_H

#include "sis_core.h"
#include "sis_math.h"

#include "sis_malloc.h"

#include "sis_table.h"
#include "sis_fields.h"
/////////////////////////////////////////////////////////
//  ���ݿ���������ģʽ
/////////////////////////////////////////////////////////
#define SIS_SEARCH_NONE -1 // û�����ݷ�������
#define SIS_SEARCH_NEAR  1  // ����������
#define SIS_SEARCH_LEFT  2  // ����������
#define SIS_SEARCH_RIGHT 3  // ����������
#define SIS_SEARCH_OK    0	// ׼ȷƥ�������

#define SIS_SEARCH_CHECK_INIT  0  // �������¼�¼���ȼ򵥰��������ж�
#define SIS_SEARCH_CHECK_NEW   1   // ����������¼��������ʱ��������һ����¼
#define SIS_SEARCH_CHECK_OLD   2   // ��¼��������ʱ��С�����һ����¼
#define SIS_SEARCH_CHECK_OK    3   // �������һ����¼
#define SIS_SEARCH_CHECK_ERROR 4 // ���󣬲�����

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
typedef struct s_sis_collect_unit
{
	s_sis_table *father;		// ���ָ�룬���Ի���ֶζ���������Ϣ
	s_sis_step_index *stepinfo; // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	s_sis_struct_list *value;   // �ṹ������

	s_sis_sds front;  // ǰһ���ӵļ�¼ catch=true��Ч volΪȫ�� -- ����ʱһ��Ҫ����
	s_sis_sds lasted; // ��ǰ��һ���ӵļ�¼ catch=true��Ч volΪȫ�� -- ����ʱһ��Ҫ����

	s_sis_sds refer;  // ʵ�����ݵ�ǰһ���ο����� zip ʱ��Ч volΪ���� -- ��Ҫ����
	// s_sis_sds moved;  // ��ǰ���ݣ�ֱ�Ӵ���������ȡ
} s_sis_collect_unit;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_step_index --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sis_step_index *sis_stepindex_create();
void sis_stepindex_destroy(s_sis_step_index *);
void sis_stepindex_rebuild(s_sis_step_index *, uint64 left_, uint64 right_, int count_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_collect_unit --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sis_collect_unit *sis_collect_unit_create(s_sis_table *tb_, const char *key_);
void sis_collect_unit_destroy(s_sis_collect_unit *);

uint64 sis_collect_unit_get_time(s_sis_collect_unit *unit_, int index_);

int sis_collect_unit_recs(s_sis_collect_unit *unit_);
int sis_collect_unit_search(s_sis_collect_unit *unit_, uint64 index_);
//����Ƿ����Ӽ�¼��ֻ�����һ����¼���Ƚϣ�����3����һ�ǵ������¼�¼��һ���¼�¼��һ���ϼ�¼
int sis_collect_unit_search_check(s_sis_collect_unit *unit_, uint64 index_);
int sis_collect_unit_search_left(s_sis_collect_unit *unit_, uint64 index_, int *mode_);
int sis_collect_unit_search_right(s_sis_collect_unit *unit_, uint64 index_, int *mode_);

int sis_collect_unit_delete_of_range(s_sis_collect_unit *, int start_, int stop_);  // ��λ��ɾ��
int sis_collect_unit_delete_of_count(s_sis_collect_unit *, int start_, int count_); // ��λ��ɾ��

s_sis_sds sis_collect_unit_get_of_range_sds(s_sis_collect_unit *, int start_, int stop_);
s_sis_sds sis_collect_unit_get_of_count_sds(s_sis_collect_unit *, int start_, int count_);
//  ����Ʊ����ֱ�Ӵӱ��л�ȡһ������
s_sis_sds sis_table_get_of_range_sds(s_sis_table *tb_, const char *code_, int start_, int stop_);

int sis_collect_unit_update(s_sis_collect_unit *, const char *in_, size_t ilen_);
// �Ӵ��̼��أ�����д�룬
int sis_collect_unit_update_block(s_sis_collect_unit *, const char *in_, size_t ilen_);

//����json����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sis_collect_json_to_struct_sds(s_sis_collect_unit *, const char *in_, size_t ilen_);

//����array����ʱͨ���ú���ת�ɶ����ƽṹ����
s_sis_sds sis_collect_array_to_struct_sds(s_sis_collect_unit *, const char *in_, size_t ilen_);

//�������ʱ���Ѷ����ƽṹ����ת����json��ʽ���ݣ�����array�����ݣ�json ����Ҫ���fields�ṹ
s_sis_sds sis_collect_struct_filter_sds(s_sis_collect_unit *unit_, s_sis_sds in_, const char *fields_);
s_sis_sds sis_collect_struct_to_json_sds(s_sis_collect_unit *unit_, s_sis_sds in_, const char *fields_);
s_sis_sds sis_collect_struct_to_array_sds(s_sis_collect_unit *unit_, s_sis_sds in_, const char *fields_);

void sis_collect_struct_trans(s_sis_sds ins_, s_sis_field_unit *infu_, s_sis_table *indb_, s_sis_sds outs_, s_sis_field_unit *outfu_, s_sis_table *outdb_);
// void sis_collect_struct_trans_incr(s_sis_sds ins_,s_sis_sds dbs_, s_sis_field_unit *infu_, s_sis_table *indb_, s_sis_sds outs_, s_sis_field_unit *outfu_,s_sis_table *outdb_);

bool sis_trans_of_count(s_sis_collect_unit *unit_, int *start_, int *count_);
bool sis_trans_of_range(s_sis_collect_unit *unit_, int *start_, int *stop_);


#endif /* _SIS_COLLECT_H */
