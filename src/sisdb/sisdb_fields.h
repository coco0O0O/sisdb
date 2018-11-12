
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_FIELDS_H
#define _SISDB_FIELDS_H

#include "sis_str.h"
#include "sis_map.h"

#include "sis_malloc.h"
#include "sisdb_table.h"

//--------32�����ͣ�����4������-------//

#pragma pack(push, 1)

#define SIS_FIELD_MAXLEN 32

typedef struct s_sisdb_field_flags
{
	unsigned type : 5;      // ��������  һ��32����������
	unsigned len : 7;       // �ֶγ���(���ֽ�)  0...63 ��󲻳���64���ַ�������˾�����������
	unsigned dot : 4;       // С���� ���15��С��λ
} s_sisdb_field_flags;

typedef struct s_sisdb_field
{
	uint8  index;				 // λ�ã��ṹ�еڼ����ֶ� ���64���ֶ�
	uint16 offset;				 // ƫ��λ�ã�ƫ�ƶ����ֽ�
	char name[SIS_FIELD_MAXLEN]; // �ֶ�����
	s_sisdb_field_flags flags;	 // attribute�ֶ�����
	//----������Щ���ܣ����������������ˣ��Ժ���˵----//
	uint8 subscribe_method;					// ��fields-catch����ʱ���ɸ��ֶ��ж���ѡ����
	char  subscribe_refer_fields[SIS_FIELD_MAXLEN]; //subscribe_refer_fields ʱ�ο����ֶ�����
} s_sisdb_field;

//��table��˵������һ��mapָ��һ�����¼sis_fieldsis_t���ݽṹ

#pragma pack(pop)

//�û������ֶζ���ʱ�Ľṹ������������ȷ������

s_sisdb_field *sisdb_field_create(int index, const char *name_, s_sisdb_field_flags *flags_);
void sisdb_field_destroy(s_sisdb_field *);

bool sisdb_field_is_time(s_sisdb_field *unit_);

#define sisdb_field_is_whole(f) (!f || !strncmp(f, "*", 1))

uint64 sisdb_field_get_uint(s_sisdb_field *unit_, const char *val_);
int64 sisdb_field_get_int(s_sisdb_field *unit_, const char *val_);
double sisdb_field_get_float(s_sisdb_field *unit_, const char *val_, int dot_);

// ʵ����ֻ��д��val�У�
void sisdb_field_set_uint(s_sisdb_field *unit_, char *val_, uint64 u64_);
void sisdb_field_set_int(s_sisdb_field *unit_, char *val_, int64 i64_);
void sisdb_field_set_float(s_sisdb_field *unit_, char *val_, double f64_, int dot_);

//�������ֻ�ȡ�ֶ�
s_sisdb_field *sisdb_field_get_from_key(s_sisdb_table *tb_, const char *key_);
int sisdb_field_get_offset(s_sisdb_table *tb_, const char *key_);

const char * sisdb_field_get_char_from_key(s_sisdb_table *tb_, const char *key_, const char *val_, size_t *len_);
uint64 sisdb_field_get_uint_from_key(s_sisdb_table *tb_, const char *key_, const char *val_);
// int64 sisdb_field_get_int_from_key(s_sisdb_table *tb_, const char *key_, const char *val_);
// double sisdb_field_get_float_from_key(s_sisdb_table *tb_, const char *key_, const char *val_);

void sisdb_field_set_uint_from_key(s_sisdb_table *tb_, const char *key_, char *val_, uint64 u64_);
// void sisdb_field_set_int_from_key(s_sisdb_table *tb_, const char *key_, char *val_, int64 i64_);
// void sisdb_field_set_double_from_key(s_sisdb_table *tb_, const char *key_, char *val_, double f64_);

void sisdb_field_copy(char *des_, const char *src_, size_t len_);

void sisdb_field_json_to_struct(s_sis_sds in_, s_sisdb_field *fu_, 
			char *key_, s_sis_json_node *node_,s_sisdb_cfg_info  *);

#endif /* _SIS_FIELDS_H */
