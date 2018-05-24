
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_FIELDS_H
#define _STS_FIELDS_H

#include "sds.h"
#include "dict.h"
#include "sts_map.h"
#include <assert.h>
/////////////////////////////////////////////////////////
//  �ֶ����Ͷ���
/////////////////////////////////////////////////////////
//����ʱ��Ķ���
#define STS_FIELD_NONE    0 // "TIDX"  //int32 ��ʾ��� 0.....1000....
#define STS_FIELD_INDEX   1 // "TIDX"  //int32 ��ʾ��� 0.....1000....
#define STS_FIELD_SECOND  2 // "TSEC"  //int32 time_t��ʽ����ȷ����  
#define STS_FIELD_MINUTE  3 // "TMIN"  //int32 time_t��ʽ / 60����ȷ����
#define STS_FIELD_MIN5    4 // "TMIN"  //int32 time_t��ʽ / 60����ȷ����
#define STS_FIELD_DAY     5 // "TDAY"  //int32 20170101��ʽ����ȷ����
#define STS_FIELD_MONTH   6 // "TMON"  //int32 201703��ʽ����ȷ����
#define STS_FIELD_YEAR    7 // "TYEAR"  //int16 2017��ʽ����ȷ����
#define STS_FIELD_TIME    8// "TTIME"  //int64 ��ȷ�������ʱ��  
//8λ���붨��
#define STS_FIELD_CODE    9 // "TCODE"  //int64 -- char[8] 8λ�ַ�ת��Ϊһ��int64Ϊ����
//�������Ͷ���
#define STS_FIELD_CHAR    10 // "CHAR"  //string ���� �����������;
//�����ʽΪ field����:��������:����; STS_FIELD_CHAR�����Ĭ��Ϊ16;
#define STS_FIELD_INT1    11 // "INT1"  //int ���� 
#define STS_FIELD_INT2    12 // "INT2"  //int ���� 
#define STS_FIELD_INT4    13 // "INT4"  //int ���� 
#define STS_FIELD_INT8    14 // "INT8"  //int ���� 
#define STS_FIELD_INTZ    15 // "INTZ"  //int ���� ѹ����������
//--------------------------------------------------------//
// �������Ͷ���Ĺ�������Լ۸�ͳɽ����Ķ��壬�������Ǹ��������ߴ���������Ҫ����info��Ϣ���зŴ����С
// �û����������ݶ���ԭʼ���ݣ�����۸��ϴ�Ϊ�����������Ǵ洢��ʱ��
// ��Ҫ����info.SH600600 �ж����С����λ���Ը����ݽ��зŴ�ȡ������ʱ�Ѷ�Ӧ������С�󷵻ظ��û���
#define STS_FIELD_PRC     18  // "PRC"  //����������ȣ�Ĭ��4����󲻳���8;
// �����json���ݸ�ʽ��Ҫת��
// ��Լ۸�float��double���洢����ǰ��Ҫ��Թ�Ʊ���뵽code���л�ȡ��Ӧ��prc_decimal 
// Ȼ�����Դ��json���ݽ��зŴ�Ȼ��ȡ�����棬ȡ����ʱ���Ҫ��תΪjson�Ͷ�ӦתΪ������
// prc_decimalȡֵ��Χ�� 0...15,����С��λ,0��ʾ���ݲ�������ֱ��ȡ������
#define STS_FIELD_VOL     19  // "VOL"  //����������ȣ�Ĭ��4����󲻳���8;
// �����json���ݸ�ʽ��Ҫת��
// ��Գɽ���int32��int64���洢����ǰ��Ҫ��Թ�Ʊ���뵽code���л�ȡ��Ӧ��vol_zoomout 
// Ȼ�����Դ��json���ݽ�����С��ȡ�����棬ȡ����ʱ���Ҫ��תΪjson�Ͷ�Ӧ�Ŵ�
// vol_zoomoutȡֵ��Χ�� 0..15 Ĭ�ϲ����ţ����Ŵ�10^15

#define STS_FIELD_FLOAT   20 // "FLOAT"  //float 
#define STS_FIELD_DOUBLE  21 // "DOUBLE"  //double

//--------32�����ͣ�����4������-------//

#pragma pack(push,1)
#if 0
typedef struct sts_int_packed{
	unsigned len : 3;   // �ֶγ���(���ֽ�)  0:1,1:2,2:4,3:8,4:16,5:32,6:64,7:128
	unsigned zoom : 5;  // �Ŵ���С����,  
	// 0x00000 0x10000 ��������
	// 0x00001...0x01111 0x0001:��С10��1λС����,0x0010:��С100�� ... 
	//                   0x1111:����С10^15����С�����15λ ����ҪΪ֧�ֱ��رң�
	//                   ����32λ������Χ��������С������С��Ҳ�޷���ʾ����ʱ����������ҪΪint64
	// 0x10001...0x11111 0x10001:�Ŵ�10��,0x10010:�Ŵ�100��, ... ���Ŵ�10^15��
}sts_int_packed;

typedef struct sts_fields_packed{
	unsigned type : 5;      // ��������  һ��32����������
	unsigned encoding : 3;  // 8�ֱ��뷽ʽ 
	union {
		sts_int_packed iflag;
		uint8_t slen;   //��������Ϊ�ַ���ʱ����һ���ֽ�Լ��ʾ�ַ������ȣ��������£�
		// 0 ... 127 ��ʾ 1...128���ֽڳ���
		// 128...255 ���ȼ��㹫ʽΪ =��128-127��*255�������Ա�ʾ����Ϊ32640���ֽڳ��ȣ�������ͨ��������
	};
}sts_fields_packed;
#endif

// typedef struct s_sts_fields_flags{
// 	unsigned type : 5;      // ��������  һ��32����������
// 	unsigned encoding : 3;  // 8�ֱ��뷽ʽ
// 	unsigned len : 6;       // �ֶγ���(���ֽ�)  0...63 ��󲻳���64���ַ�������˾�����������
// 	unsigned group : 2;     // һ����¼ͬһ������������Ϊ4�飬һ���ò�����
// }s_sts_fields_flags;
// typedef struct s_sts_field_unit{
// 	uint8_t  index;   // λ�ã��ṹ�еڼ����ֶ� ���64���ֶ�
// 	uint16_t offset;  // ƫ��λ�ã��ṹ�еڼ����ֶΣ�ƫ�ƶ����ֽ�
// 	char name[STS_FIELD_MAXLEN];  // �ֶ�����
// 	s_sts_fields_flags flags; // attribute�ֶ�����
// }s_sts_field_unit;

typedef struct s_sts_fields_flags{
	unsigned type : 5;    // ��������  һ��32����������
	unsigned len : 6;     // �ֶγ���(���ֽ�)  0...63 ��󲻳���64���ַ�������˾�����������
	unsigned io : 1;      // 0 ����ʱ�Ŵ� ���ʱ��С��float����ȡС���� int ȡ��
	unsigned zoom : 4;    // zoom = 0 �����ţ�����ioΪ��ֵ��ʵ�ʱ�������ݶ���int32���� 
	// io = 0 ����ʱ����zoom��ȡ�������ʱ����zoom������С������� ��float&double��Ч
	// ���֧��15λС��, 
	// io = 1 ����ʱ����zoom��ȡ�������ʱ����zoom��ȡ�� ��int��Ч
	// ���֧��10^15�η�,�㹻��ʾ������ 
	unsigned ziper : 2;  // none ��ѹ�� up ��һ�� local �͵�ǰ��¼ĳ�ֶαȽ�ѹ�� multi ��ָ���ֶ����
	unsigned refer : 6;  // ��Ϊup��local��ʱ�򣬱�ʾ�͵ڼ����ֶαȽ�ѹ����local��������Ϊ�Լ�ѹ�Լ�
				// ziper==refer==0 ��ʾ��ѹ�� 
}s_sts_fields_flags;

#define STS_FIELD_MAXLEN  32

typedef struct s_sts_field_unit{
	uint8_t  index;   // λ�ã��ṹ�еڼ����ֶ� ���64���ֶ�
	uint16_t offset;  // ƫ��λ�ã��ṹ�еڼ����ֶΣ�ƫ�ƶ����ֽ�
	char name[STS_FIELD_MAXLEN];  // �ֶ�����
	s_sts_fields_flags flags; // attribute�ֶ�����
}s_sts_field_unit;

//��table��˵������һ��mapָ��һ�����¼sts_field_unit���ݽṹ

#pragma pack(pop)


//�û������ֶζ���ʱ�Ľṹ������������ȷ������
//ʾ����"fields":[{"name":"time","type":"TT","zip":"ROW"},
//				  {"name":"close","type":"FL","zip":"ALL"},
//                {"name":"vol","type":"I8","zip":"ROW"},
//                {"name":"name","type":"CH","len":16}];
//
//     �����ɵ�1��ʱ���ֶ�time������8B������ѹ��
//			 ��2���ֶ�Ϊ�����ֶ�close��ֵ����100��������λС��,ѹ����ʽΪall
//           ��3��Ϊ�����ֶ�vol������8B  ����ѹ��
//			 ��2��Ϊ�ַ���name������Ϊ16 ��ѹ��

s_sts_field_unit *sts_field_unit_create(int index, const char *name_, const char *type_, const char *zip, int len_, int group_);
void sts_field_unit_destroy(s_sts_field_unit *);

bool sts_field_is_times(int t_);

#endif  /* _STS_FIELDS_H */
