
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_FIELDS_H
#define _STS_FIELDS_H

#include "sds.h"
#include "dict.h"
#include <assert.h>
#include "sdsalloc.h"
#include "sts_str.h"
#include "sts_map.h"


/////////////////////////////////////////////////////////
//  �ֶ����Ͷ���
/////////////////////////////////////////////////////////
//����ʱ��Ķ���
#define STS_FIELD_NONE    0 // "NONE"  //
#define STS_FIELD_INDEX   1 // "INDEX"  //int32 ��ʾ��� 0.....1000....
#define STS_FIELD_SECOND  2 // "SECOND"  //int32 time_t��ʽ����ȷ����  
#define STS_FIELD_MIN1    3 // "MIN1"  //int32 time_t��ʽ / 60����ȷ����
#define STS_FIELD_MIN5    4 // "MIN5"  //int32 time_t��ʽ / 60����ȷ����
#define STS_FIELD_DAY     5 // "DAY"  //int32 20170101��ʽ����ȷ����
#define STS_FIELD_TIME    6 //"TIME"  //int64 ����
//8λ���붨��
#define STS_FIELD_CODE    9 // "CODE"  //int64 -- char[8] 8λ�ַ�ת��Ϊһ��int64Ϊ����
//�������Ͷ���
#define STS_FIELD_STRING  10 // "STRING"  //string ���� �����������;
//�����ʽΪ field����:��������:����; STS_FIELD_STRING�����Ĭ��Ϊ16;
#define STS_FIELD_INT     11 // "INT"    //int ���� 
#define STS_FIELD_UINT    12 // "UINT"    //unsigned int ���� 
#define STS_FIELD_FLOAT   13 // "FLOAT"  //float 
#define STS_FIELD_DOUBLE  14 // "DOUBLE" //double

//--------32�����ͣ�����4������-------//

#pragma pack(push,1)

typedef struct s_sts_fields_flags{
	unsigned type : 4;    // ��������  һ��16����������
	unsigned len : 6;     // �ֶγ���(���ֽ�)  0...63 ��󲻳���64���ַ�������˾�����������
	unsigned io : 1;      // 0 ����ʱ�Ŵ� ���ʱ��С������ʱΪstruct�Ͳ��ٷŴ��� float����ȡС���� int ȡ��
	unsigned zoom : 4;    // zoom = 0 �����ţ�����ioΪ��ֵ��ʵ�ʱ�������ݶ���int32���� 
	// io = 0 ����ʱ����zoom��ȡ�������ʱ����zoom������С������� ��float&double��Ч
	// ���֧��15λС��, 
	// io = 1 ����ʱ����zoom��ȡ�������ʱ����zoom��ȡ�� ��int��Ч
	// ���֧��10^15�η�,�㹻��ʾ������ 
	unsigned ziper : 3;  // none ��ѹ�� up ��һ�� local �͵�ǰ��¼ĳ�ֶαȽ�ѹ�� multi ��ָ���ֶ����
	unsigned refer : 6;  // ��Ϊup��local��ʱ�򣬱�ʾ�͵ڼ����ֶαȽ�ѹ����local��������Ϊ�Լ�ѹ�Լ�
				// ziper==refer==0 ��ʾ��ѹ��  64���ֶ�
}s_sts_fields_flags;

#define STS_FIELD_MAXLEN  32

typedef struct s_sts_field_unit{
	uint8_t  index;   // λ�ã��ṹ�еڼ����ֶ� ���64���ֶ�
	uint16 offset;  // ƫ��λ�ã��ṹ�еڼ����ֶΣ�ƫ�ƶ����ֽ�
	char name[STS_FIELD_MAXLEN];  // �ֶ�����
	s_sts_fields_flags flags; // attribute�ֶ�����
}s_sts_field_unit;

//��table��˵������һ��mapָ��һ�����¼sts_field_unit���ݽṹ

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

s_sts_field_unit *sts_field_unit_create(int index, const char *name_, s_sts_fields_flags *flags_);
void sts_field_unit_destroy(s_sts_field_unit *);

bool sts_field_is_times(int t_);

#define sts_check_fields_all(f) (!f || !strncmp(f, "*", 1))

uint64 sts_fields_get_uint(s_sts_field_unit *fu_, const char *val_);
int64 sts_fields_get_int(s_sts_field_unit *fu_, const char *val_);
double sts_fields_get_double(s_sts_field_unit *fu_, const char *val_);

#endif  /* _STS_FIELDS_H */
