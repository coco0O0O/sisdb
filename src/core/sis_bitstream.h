#ifndef _SIS_BITSTREAM_H
#define _SIS_BITSTREAM_H

#include "sis_core.h"
#include "sis_zint.h"

//m_method_c
//'B':���ݲ�����m_data_dwλ, 
//'b':���ݲ�����m_data_dwλ, ��������
//'m':���ݲ�����m_data_dwλ, ����
//'S':��������HIWORD(m_data_dw)�󲻳���LOWORD(m_data_dw)λ
//'E':���ݵ���CodeData
//'Z':����Ϊx*10^N, x<CodeData,N==1-4
//'P':��m_data_dwλ�����У�����һ��1�����1��λ��
//'D':ԭʼ�������ݣ�ԭʼ���ݷ���Bit��,���Ƿ�������棬������ƥ��
//'F':ԭʼ�������ݣ�ԭʼ���ݷ���Bit��,���Ƿ�������棬������ƥ��
//'M':ԭʼzint32���ݣ�ԭʼ���ݷ���Bit��,���Ƿ�������棬������ƥ��

#pragma pack(push,1)

typedef struct s_sis_bit_code{
	uint16	m_sign_w;	    //������
	uint8	m_len_by;		//���볤��
	uint8	m_data_len_by;	//������������ݳ���,��λBit�����32Bit
	char	m_method_c;		//���뷽��
	uint32	m_data_dw;	         //��������
	uint32	m_data_offset_dw;	//��������ƫ����
}s_sis_bit_code;

typedef struct s_sis_bit_stream{
	uint8*    m_stream;				//������
	uint32    m_bit_size_i;			//��������С��Bit

	uint32    m_nowpos_i;			//��ǰλ��
	uint32    m_savepos_i;			//��һ�α����λ��

	const   s_sis_bit_code  *m_nowcode_p;    //��ǰ��������
	int		m_nowcode_count;	     //��ǰ���������¼��
}s_sis_bit_stream;

#pragma pack(pop)

#define SETBITCODE(_s_,_b_) { ((s_sis_bit_stream *)_s_)->m_nowcode_p = &_b_[0]; ((s_sis_bit_stream *)_s_)->m_nowcode_count = sizeof(_b_)/sizeof(_b_[0]); }

// #define SETBITCODE(_s_,_b_) {  }

s_sis_bit_stream *sis_bitstream_create(uint8 *in_, size_t inLen_, uint32 initPos_);
void sis_bitstream_destroy(s_sis_bit_stream *);

void sis_bitstream_link(s_sis_bit_stream *s_, uint8 *in_, uint32 inLen_, uint32 initPos_);
int  sis_bitstream_getbytelen(s_sis_bit_stream *s_);//�õ���ǰBit��Byte��

int    sis_bitstream_get_nomove(s_sis_bit_stream *s_, int nBit, uint32 *out);
uint32 sis_bitstream_get(s_sis_bit_stream *s_, int nBit);
void   sis_bitstream_move(s_sis_bit_stream *s_, int byte);
int    sis_bitstream_get_string(s_sis_bit_stream *s_, char* out_, int outLen_);
int    sis_bitstream_get_buffer(s_sis_bit_stream *s_, char* out_, int outLen_);
int    sis_bitstream_moveto(s_sis_bit_stream *s_, int nPos);

int sis_bitstream_put(s_sis_bit_stream *s_, uint32 dw, int nBit);
int sis_bitstream_put_string(s_sis_bit_stream *s_, const char * str);
int sis_bitstream_put_buffer(s_sis_bit_stream *s_, char *in, size_t len);

const s_sis_bit_code* sis_bitstream_find_original_data(s_sis_bit_stream *s_);
const s_sis_bit_code* sis_bitstream_find_string_data(s_sis_bit_stream *s_);
const s_sis_bit_code* sis_bitstream_decode_find_match(s_sis_bit_stream *s_, uint32 *out_);

int sis_bitstream_encode_uint32(s_sis_bit_stream *s_, uint32 dwData, const uint32 *pdwLastData, bool bReverse);
int sis_bitstream_encode_zint32(s_sis_bit_stream *s_, zint32 mData, const zint32* pmLastData);
int sis_bitstream_encode_float(s_sis_bit_stream *s_, float f, bool check0);
int sis_bitstream_encode_string(s_sis_bit_stream *s_, const char * strData, uint32 *base);

uint32 sis_bitstream_decode_uint32(s_sis_bit_stream *s_, const uint32* base, bool bReverse);
void sis_bitstream_decode_zint32(s_sis_bit_stream *s_, const zint32* base, zint32 *out, bool bReverse);
float sis_bitstream_decode_float(s_sis_bit_stream *s_, bool check0);
uint32 sis_bitstream_decode_string(s_sis_bit_stream *s_, char* in_, int inLen_, uint32 *base_);						//����������Label���򷵻ظ����֣����򷵻�0xFFFFFFFF,����dwLastData

#endif //_LW_BITSTREAM_H

