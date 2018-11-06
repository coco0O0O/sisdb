
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

typedef struct s_sis_fields_flags
{
	unsigned type : 4; // ��������  һ��16����������
	unsigned len : 6;  // �ֶγ���(���ֽ�)  0...63 ��󲻳���64���ַ�������˾�����������
	unsigned io : 1;   // 0 ����ʱ�Ŵ� ���ʱ��С������ʱΪstruct�Ͳ��ٷŴ��� float����ȡС���� int ȡ��
	unsigned zoom : 4; // zoom = 0 �����ţ�����ioΪ��ֵ��ʵ�ʱ�������ݶ���int32����
	// io = 0 ����ʱ����zoom��ȡ�������ʱ����zoom������С������� ��float&double��Ч
	// ���֧��15λС��,
	// io = 1 ����ʱ����zoom��ȡ�������ʱ����zoom��ȡ�� ��int��Ч
	// ���֧��10^15�η�,�㹻��ʾ������
	unsigned ziper : 3; // 0 ��ѹ�� up ��һ�� local �͵�ǰ��¼ĳ�ֶαȽ�ѹ�� multi ��ָ���ֶ����
	unsigned refer : 6; // ��Ϊup��local��ʱ�򣬱�ʾ�͵ڼ����ֶαȽ�ѹ����local��������Ϊ�Լ�ѹ�Լ�
		// ziper==refer==0 ��ʾ��ѹ��  64���ֶ�
} s_sis_fields_flags;

#define SIS_FIELD_MAXLEN 32


typedef struct s_sis_field_unit
{
	uint8_t index;				 // λ�ã��ṹ�еڼ����ֶ� ���64���ֶ�
	uint16 offset;				 // ƫ��λ�ã�ƫ�ƶ����ֽ�
	char name[SIS_FIELD_MAXLEN]; // �ֶ�����
	s_sis_fields_flags flags;	 // attribute�ֶ�����
	//----������Щ���ܣ����������������ˣ��Ժ���˵----//
	uint8_t catch_method;					// ��fields-catch����ʱ���ɸ��ֶ��ж���ѡ����
	char catch_initfield[SIS_FIELD_MAXLEN]; //catch_initfield ʱ�ο����ֶ�����
} s_sis_field_unit;

//��table��˵������һ��mapָ��һ�����¼sis_fieldsis_t���ݽṹ

#pragma pack(pop)

//�û������ֶζ���ʱ�Ľṹ������������ȷ������
//   fields: [

//   # ------ io & zoom ˵�� -------
//   # io = 0 ����ʱ����zoom��ȡ�������ʱ����zoom������С������� ��float&double��Ч
//   # ���֧��15λС��,
//   # io = 1 ����ʱ����zoom��ȡ�������ʱ����zoom��ȡ�� ��int��Ч
//   # ���֧��10^15�η�,�㹻��ʾ������

//   # �ֶ���| ��������| ����| io �Ŵ�����С| ���ű��� zoom|ѹ������|ѹ���ο��ֶ�����

//   [name, string, 16, 0, 0, 0, 0],
//   [type, int, 1, 0, 0, 0, 0],  # ��Ʊ����
//   [decimal, int, 1, 0, 0, 0, 0], # �۸�С����
//   [volunit, int, 1, 0, 0, 0, 0], # �ɽ�����λ
//   [before, int, 4, 0, 2, local, before],
//   [stophigh, int, 4, 0, 2, local, before],
//   [stoplow, int, 4, 0, 2, local, before]
//   ]

s_sis_field_unit *sis_field_unit_create(int index, const char *name_, s_sis_fields_flags *flags_);
void sis_field_unit_destroy(s_sis_field_unit *);

bool sis_field_is_time(s_sis_field_unit *unit_);

#define sis_check_fields_all(f) (!f || !strncmp(f, "*", 1))

uint64 sis_fields_get_uint(s_sis_field_unit *fu_, const char *val_);
int64 sis_fields_get_int(s_sis_field_unit *fu_, const char *val_);
double sis_fields_get_double(s_sis_field_unit *fu_, const char *val_);

void sis_fields_set_uint(s_sis_field_unit *fu_, char *val_, uint64 u64_);
void sis_fields_set_int(s_sis_field_unit *fu_, char *val_, int64 i64_);
void sis_fields_set_double(s_sis_field_unit *fu_, char *val_, double f64_);

//�������ֻ�ȡ�ֶ�
s_sis_field_unit *sis_field_get_from_key(s_sis_table *tb_, const char *key_);
int sis_field_get_offset(s_sis_table *tb_, const char *key_);

const char * sis_fields_get_string_from_key(s_sis_table *tb_, const char *key_, const char *val_, size_t *len_);
uint64 sis_fields_get_uint_from_key(s_sis_table *tb_, const char *key_, const char *val_);
int64 sis_fields_get_int_from_key(s_sis_table *tb_, const char *key_, const char *val_);
double sis_fields_get_double_from_key(s_sis_table *tb_, const char *key_, const char *val_);

void sis_fields_set_uint_from_key(s_sis_table *tb_, const char *key_, char *val_, uint64 u64_);
void sis_fields_set_int_from_key(s_sis_table *tb_, const char *key_, char *val_, int64 i64_);
void sis_fields_set_double_from_key(s_sis_table *tb_, const char *key_, char *val_, double f64_);

void sis_fields_copy(char *des_, const char *src_, size_t len_);

#endif /* _SIS_FIELDS_H */
