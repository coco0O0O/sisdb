
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_COLLECT_H
#define _STS_COLLECT_H

#include "sts_core.h"
#include "sds.h"
#include "sts_math.h"
// #include "zmalloc.h"
// #include "sdsalloc.h"

#include "sts_table.h"

/////////////////////////////////////////////////////////
//  ���ݿ���������ģʽ
/////////////////////////////////////////////////////////
#define STS_SEARCH_NONE     -1  // û�����ݷ�������
#define STS_SEARCH_NEAR      1  // ����������
#define STS_SEARCH_LEFT      2  // ����������
#define STS_SEARCH_RIGHT     3  // ����������
#define STS_SEARCH_OK        0  // ׼ȷƥ�������

#define STS_SEARCH_CHECK_INIT   0  // �������¼�¼���ȼ򵥰��������ж�
#define STS_SEARCH_CHECK_NEW    1  // ����������¼��������ʱ��������һ����¼
#define STS_SEARCH_CHECK_OLD    2  // ��¼��������ʱ��С�ڻ�������һ����¼
#define STS_SEARCH_CHECK_ERROR  3  // ���󣬲�����

#define STS_JSON_KEY_ARRAY    ("value")   
#define STS_JSON_KEY_ARRAYS   ("values")   
#define STS_JSON_KEY_GROUPS   ("groups")   
#define STS_JSON_KEY_FIELDS   ("fields")   

#pragma pack(push,1)
// ���ݽṹ�������ʱ�����У��Զ�����ͷβʱ�䣬��ƽ�����ʱ��
typedef struct s_sts_step_index {
	uint64 left;       // ��Сʱ��
	uint64 right;   
	int count;
	uint64 step;       // ���ʱ�䣬ÿ����¼��Լ���ʱ�䣬
}s_sts_step_index;

// ������Ʊ�����ݰ�
typedef struct s_sts_collect_unit{
	s_sts_table        *father;  // ���ָ�룬���Ի���ֶζ���������Ϣ
	s_sts_step_index   *stepinfo;    // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	s_sts_struct_list  *value;   // �ṹ������

	sds  front;   // ǰһ���ӵļ�¼ catch=true��Ч -- ����ʱһ��Ҫ����
	sds  lasted; // ��ǰ��һ���ӵļ�¼ catch=true��Ч -- ����ʱһ��Ҫ����
	sds  moved;   // �ƶ��е�
}s_sts_collect_unit;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////
//------------------------s_sts_step_index --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sts_step_index *sts_stepindex_create();
void sts_stepindex_destroy(s_sts_step_index *);
void sts_stepindex_rebuild(s_sts_step_index *, uint64 left_, uint64 right_, int count_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_sts_collect_unit --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_sts_collect_unit *sts_collect_unit_create(s_sts_table *tb_, const char *key_);
void sts_collect_unit_destroy(s_sts_collect_unit *);

uint64 sts_collect_unit_get_time(s_sts_collect_unit *unit_, int index_);

int	sts_collect_unit_recs(s_sts_collect_unit *unit_);
int sts_collect_unit_search(s_sts_collect_unit *unit_, uint64 index_);
//����Ƿ����Ӽ�¼��ֻ�����һ����¼���Ƚϣ�����3����һ�ǵ������¼�¼��һ���¼�¼��һ���ϼ�¼
int sts_collect_unit_search_check(s_sts_collect_unit *unit_, uint64 index_);
int sts_collect_unit_search_left(s_sts_collect_unit *unit_, uint64 index_, int *mode_);
int sts_collect_unit_search_right(s_sts_collect_unit *unit_, uint64 index_, int *mode_);

int	sts_collect_unit_delete_of_range(s_sts_collect_unit *, int start_, int stop_); // ��λ��ɾ��
int	sts_collect_unit_delete_of_count(s_sts_collect_unit *, int start_, int count_); // ��λ��ɾ��

sds sts_collect_unit_get_of_range_m(s_sts_collect_unit *, int start_, int stop_);
sds sts_collect_unit_get_of_count_m(s_sts_collect_unit *, int start_, int count_);

int sts_collect_unit_update(s_sts_collect_unit *, const char *in_, size_t ilen_);

//����json����ʱͨ���ú���ת�ɶ����ƽṹ����
sds sts_collect_json_to_struct(s_sts_collect_unit *, const char *in_, size_t ilen_);

//����array����ʱͨ���ú���ת�ɶ����ƽṹ����
sds sts_collect_array_to_struct(s_sts_collect_unit *, const char *in_, size_t ilen_);

//�������ʱ���Ѷ����ƽṹ����ת����json��ʽ���ݣ�����array�����ݣ�json ����Ҫ���fields�ṹ
sds sts_collect_struct_filter(s_sts_collect_unit *unit_, sds in_, const char *fields_);
sds sts_collect_struct_to_json(s_sts_collect_unit *unit_, sds in_, const char *fields_);
sds sts_collect_struct_to_array(s_sts_collect_unit *unit_, sds in_, const char *fields_);

void sts_collect_struct_trans(sds ins_, s_sts_field_unit *infu_, s_sts_table *indb_, sds outs_, s_sts_field_unit *outfu_,s_sts_table *outdb_);
// void sts_collect_struct_trans_incr(sds ins_,sds dbs_, s_sts_field_unit *infu_, s_sts_table *indb_, sds outs_, s_sts_field_unit *outfu_,s_sts_table *outdb_);

#endif  /* _STS_COLLECT_H */
