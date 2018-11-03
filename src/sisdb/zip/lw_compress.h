#ifndef COMPRESS_DAY_H
#define COMPRESS_DAY_H

#include "clpub.h"
#include "compress.h"
#include "memory.h"


#ifdef __cplusplus
extern "C" {
#endif

	listNode *cl_redis_compress_day(uint8 *in, int count, s_stock_static *info);
	listNode *cl_redis_compress_close(uint8 *in, int count_, s_stock_static *info);

	listNode *cl_redis_compress_min(uint8 *in, int count, s_stock_static *info);

	listNode *cl_redis_compress_tmin(uint8 *in, int count, s_stock_static *info); //---���շ���������Ϊ������int����Ҫ���⴦��
	listNode *cl_redis_compress_tmin_new(listNode *src, uint8 *in, int count, s_stock_static *info);
	//������봫������ʵʱ���������ݣ�������Ҫ�Ӻܶ��жϣ������㣻��ʹ����ȫ�����ݣ�Ҳֻ���޸��������ݣ�����̫�˷���Դ

	listNode *cl_redis_compress_tick(uint8 *in, int count, s_stock_static *info, bool isnow);
	listNode *cl_redis_compress_tick_to_tick(uint8 *in, int count, s_stock_static *info, bool isnow);

	//���½�ѹ��Ϊǣ�浽���������û��������ţ�[],[],[],[]
	s_memory_node *cl_redis_uncompress_day(uint8 *in, int inlen, OUT_COMPRESS_TYPE type);
	s_memory_node *cl_redis_uncompress_min(uint8 *in, int inlen, OUT_COMPRESS_TYPE type); //��ʷ�����ߣ�
	s_memory_node *cl_redis_uncompress_tmin(uint8 *in, int inlen, OUT_COMPRESS_TYPE type); //���շ����ߣ�
	s_memory_node *cl_redis_uncompress_tick(uint8 *in, int inlen, OUT_COMPRESS_TYPE type);//5�� tick
	s_memory_node *cl_redis_uncompress_tick_to_day(uint8 *in, int inlen, OUT_COMPRESS_TYPE type);//5�� day

	listNode *cl_redis_compress_dyna(uint8 *in, int count, s_stock_static *info, bool isnow);
	s_memory_node *cl_redis_uncompress_dyna(uint8 *in, int inlen, OUT_COMPRESS_TYPE type);//5�� tick

#ifdef __cplusplus
}
#endif


#endif //_DAY_COMPRESS_H_INCLUDE
