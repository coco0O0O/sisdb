#ifndef COMPRESS_H
#define COMPRESS_H

#include "clpub.h"
#include "bitstream.h"
#include "clmath.h"
#include "sdsnode.h"
#include "stkdef.h"
#include "keymap.h"
#include "memory.h"

#pragma pack(push,1)

//�����������һ���ַ�����R���ʹ��������û�б�ѹ��������ֱ�Ӵ�ӡ������
#define CL_COMPRESS_MAX_RECS 250     //ÿ�������250����¼��251..255 Ԥ��
//*****ע��*****//
//ÿ�����ݿ��С���ܳ��� SDS_NODE_SIZE//

/*����Ҫע�⣬�ṹ����˳���д��˳�����෴��*/
typedef struct cl_block_head{
	unsigned char hch; //Ĭ��R
	unsigned type : 7; //��������
	unsigned compress : 1; //0:�������� 1:��ʾ���һ�����ݿ�
	unsigned char count;   /* ��¼���������ݿ�����ݸ��������254����¼ ���==255����ʾ����2λ��ʾ��¼�������2^16����¼*/
}cl_block_head;
//cl_block_head ֮�������������һ�����һ��ͷ��׼�͸��ֵ�λ���壬����ļ�¼�͸���ͷ��׼���������ݣ�
//������Ҫ��̬ѹ����ʵʱ����,����һ������,��¼���п���Ϊ0��1
//      compress ���Ϊ0,�������ݣ�
//      compress ���Ϊ1,��ʾ���һ�����ݿ飬cl_block_head�ṹ������һ��cl_last_info�ṹ,��ʾ��һ���ݿ�����ƫ��λ��
//				 ����һ��ͷ��׼,������Ϊ�������ݵĻ�׼���ݣ������¼��Ϊ0������Ͳ��������ˣ�
//				 �����¼��Ϊ1���͸����µ����ݣ�����ʵʱ�ķ����ߣ��������������ж��ǲ��������ݣ�����Ǿ����޸���һ��������ݣ�
//				 ��������������ݣ��Ǿ�ֻ�޸����һ�����ݿ�����ݣ�
//        (���ø÷��������԰��������ݵ����������ݷ����ͻ�)����ÿ���鶼���Զ�����ѹ
typedef struct cl_last_info{
	uint16_t offset;/* ���ݿ�Ľ���ƫ���ֽ� --> �����¼�¼Aʱ���ж�offset�Ƿ����SDS_NODE_SIZE��
					������ڣ��Ϳ�ʼ��һ���ݿ飬
					������󣬾�ѹ����ǰ��¼A��
					ֻ�����һ�����ݿ鱣��ýṹ��*/
	uint8_t bit;       //offset*8 - bit Ϊʵ�ʵ�����λ����
}cl_last_info;

#pragma pack(pop)


enum CL_COMPRESS_TYPE{
	CL_COMPRESS_DAY, //[day,o,h,l,n,v,m] 
	CL_COMPRESS_D5,  //[t,n,v] 
	CL_COMPRESS_TICK,//[t,n,v] vΪ���� 
	CL_COMPRESS_M5,  //[t,o,h,l,n,v,m] 
	CL_COMPRESS_MIN, //[t,o,h,l,n,v,m] v��mΪ���� 
	CL_COMPRESS_TMIN, //[i,o,h,l,n,v,m] v��mΪ���� 
	CL_COMPRESS_DYNA, //[id,t,o,h,l,n,v,m,....] map
	CL_COMPRESS_STATIC, //[id,code,name,preclose,....]  code
	CL_COMPRESS_CLOSE, //[day,o] 
};

enum OUT_COMPRESS_TYPE{
	OUT_COMPRESS_MEM,
	OUT_COMPRESS_JSON,
};

#define SDS_TO_STRING(s,r) if(r->value) { s->set((sds)(r->value), sdslen((sds)(r->value))); }

#define SDS_MAP_BLOCK_HEAD(h,r) if(r->value) { h=(cl_block_head *)((sds)(r->value)); }

inline int get_compress_node_count(listNode *node)
{
	if (node == NULL) return 0;
	int k = 0;
	struct listNode *n = node;
	while (n != NULL)
	{
		if (n->value)
		{
			sds s = (sds)(n->value);
			if (s[0] == 'R'){
				c_bit_stream stream((uint8*)s, sdslen(s));
				stream.MoveTo(16);
				k += stream.Get(8);
			}
		}
		n = n->next;
	}
	return k;
}
class c_compress_base
{
	//����std::string �Ļ��湦�ܣ����ܷ����ַ�����Ҳ�ܷ������ݻ��棻
	//if (str.capacity<inlen) str.setsize(inlen,0); memcpy(str.data(),in,inlen);
private:
	int check_indata_len(int type_, size_t inlen);
	s_memory_node * uncompress(listNode *innode, OUT_COMPRESS_TYPE type);
public:
	c_compress_base(){ };
	~c_compress_base(){ };
	//ʹ�øú����������������ݰ�
	listNode *compress(int type_, uint8 *in, size_t inlen, s_stock_static *info);

	//listNode *make_empty(int type_);
	//ʹ������ѹ����ʽ����Ҫ���ԭ���ݰ������Ƿ�ƥ�䣬listNode *srcnode�ͷ���ֵ������ͬһ����,�����ͷ��ڴ���ܻ����й¶
	listNode *compress_onenew_min(listNode *srcnode, uint8 *in, int inlen, s_stock_static *info);

	s_memory_node * uncompress_to_mem(listNode *innode);

	s_memory_node * uncompress_to_string(listNode *innode);

};


#endif //BASE_COMPRESS_H
