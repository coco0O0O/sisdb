#ifndef _LW_BITSTREAM_H
#define _LW_BITSTREAM_H

#include "sis_core.h"
#include "lw_zint.h"

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

typedef struct s_bit_code{
	uint16	m_sign_w;	    //������
	uint8	m_len_by;		//���볤��
	uint8	m_data_len_by;	//������������ݳ���,��λBit�����32Bit
	char	m_method_c;		//���뷽��
	uint32	m_data_dw;	    /*��������*/
	uint32	m_data_offset_dw;	//��������ƫ����
}s_bit_code;

typedef struct s_bit_stream{
	uint8*    m_stream;				//������
	uint32    m_bit_size_i;			//��������С��Bit

	uint32    m_nowpos_i;			//��ǰλ��
	uint32    m_savepos_i;			//��һ�α����λ��

	const s_bit_code  *m_nowcode_p;    //��ǰ��������
	int		m_nowcode_count;	 //��ǰ���������¼��
}s_bit_stream;

#pragma pack(pop)

bool is_original_data(const s_bit_code *bm);
// �Ƿ�ԭʼ����
bool is_bit_position(const s_bit_code *bm);
// �Ƿ��ʾ1��λ��

s_bit_stream *create_bit_stream(uint8 *in_, size_t inLen_, uint32 initPos_);
void destroy_bit_stream(s_bit_stream *);

void bit_stream_link(s_bit_stream *s_, uint8 *in_, uint32 inLen_, uint32 initPos_);
int bit_stream_getbytelen(s_bit_stream *s_);//�õ���ǰBit��Byte��

#endif //_LW_BITSTREAM_H

