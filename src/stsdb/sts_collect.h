
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_COLLECT_H
#define _STS_COLLECT_H

#include "sts_table.h"
#include "lw_map.h"
#include "lw_skiplist.h"
#include "lw_list.h"

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
typedef struct s_step_index {
	uint64 left;       // ��Сʱ��
	uint64 right;   
	int count;
	uint64 step;       // ���ʱ�䣬ÿ����¼��Լ���ʱ�䣬
}s_step_index;

// ������Ʊ�����ݰ�
typedef struct sts_collect_unit{
	sts_table      *father;  // ���ָ�룬���Ի���ֶζ���������Ϣ
	s_step_index   *step;    // ʱ������������ᱣ��ʱ������key��ÿ����¼��ָ��(�������ڴ�)��
	s_struct_list  *value;   // �ṹ������
}sts_collect_unit;

#pragma pack(pop)

///////////////////////////////////////////////////////////////////////////
//------------------------s_step_index --------------------------------//
///////////////////////////////////////////////////////////////////////////

s_step_index *create_stepindex();
void destroy_stepindex(s_step_index *);
void stepindex_rebuild(s_step_index *, uint64 left_, uint64 right_, int count_);

///////////////////////////////////////////////////////////////////////////
//------------------------sts_collect_unit --------------------------------//
///////////////////////////////////////////////////////////////////////////

sts_collect_unit *create_sts_collect_unit(sts_table *tb_, const char *key_);
void destroy_sts_collect_unit(sts_collect_unit *);

uint64 sts_collect_unit_get_time(sts_collect_unit *unit_, int index_);

int	sts_collect_unit_recs(sts_collect_unit *unit_);
int sts_collect_unit_search(sts_collect_unit *unit_, uint64 index_);
int sts_collect_unit_search_left(sts_collect_unit *unit_, uint64 index_, int *mode_);
int sts_collect_unit_search_right(sts_collect_unit *unit_, uint64 index_, int *mode_);

int	sts_collect_unit_delete_of_range(sts_collect_unit *, int start_, int stop_); // ��λ��ɾ��
int	sts_collect_unit_delete_of_count(sts_collect_unit *, int start_, int count_); // ��λ��ɾ��

sds sts_collect_unit_get_of_range_m(sts_collect_unit *, int start_, int stop_, int format_, const char *fields_);
sds sts_collect_unit_get_of_count_m(sts_collect_unit *, int start_, int count_, int format_, const char *fields_);

int sts_collect_unit_update(sts_collect_unit *, const char *in_, size_t inLen_);

#endif  /* _STS_COLLECT_H */
