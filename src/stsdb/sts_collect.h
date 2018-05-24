
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_COLLECT_H
#define _STS_COLLECT_H

#include "sts_core.h"
#include "sts_table.h"
#include "sts_map.h"
#include "sts_list.h"
// #include "zmalloc.h"
#include "sdsalloc.h"

/////////////////////////////////////////////////////////
//  ���ݿ���������ģʽ
/////////////////////////////////////////////////////////
#define STS_SEARCH_NONE     -1  // û�����ݷ�������
#define STS_SEARCH_NEAR      1  // ����������
#define STS_SEARCH_LEFT      2  // ����������
#define STS_SEARCH_RIGHT     3  // ����������
#define STS_SEARCH_OK        0  // ׼ȷƥ�������

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
	s_sts_table      *father;  // ���ָ�룬���Ի���ֶζ���������Ϣ
	//--------//
	s_sts_step_index   *step;    // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	s_sts_struct_list  *value;   // �ṹ������
	//--------//
	s_sts_string_list  *string;   // json���ַ�������
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

bool sts_collect_unit_as_struct(s_sts_collect_unit *unit_);
int	sts_collect_unit_recs(s_sts_collect_unit *unit_);
int sts_collect_unit_search(s_sts_collect_unit *unit_, uint64 index_);
int sts_collect_unit_search_left(s_sts_collect_unit *unit_, uint64 index_, int *mode_);
int sts_collect_unit_search_right(s_sts_collect_unit *unit_, uint64 index_, int *mode_);

int	sts_collect_unit_delete_of_range(s_sts_collect_unit *, int start_, int stop_); // ��λ��ɾ��
int	sts_collect_unit_delete_of_count(s_sts_collect_unit *, int start_, int count_); // ��λ��ɾ��

sds sts_collect_unit_get_of_range_m(s_sts_collect_unit *, int start_, int stop_, int format_, const char *fields_);
sds sts_collect_unit_get_of_count_m(s_sts_collect_unit *, int start_, int count_, int format_, const char *fields_);

int sts_collect_unit_update_json(s_sts_collect_unit *, const char *in_, size_t ilen_);
int sts_collect_unit_update_struct(s_sts_collect_unit *, const char *in_, size_t ilen_);

#endif  /* _STS_COLLECT_H */
