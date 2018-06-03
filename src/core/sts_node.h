#ifndef _STS_NODE_H
#define _STS_NODE_H

#include "sts_core.h"
#include "sts_malloc.h"

///////////////////////////////////////////////////////////////////////////
//------------------------s_sts_list_node --------------------------------//
//  ���� s_sts_list_node �б�ĺ���
//  value Ϊ s_sts_sds ����
///////////////////////////////////////////////////////////////////////////
s_sts_list_node *sts_sdsnode_create(const void *in, size_t inlen);
void sts_sdsnode_destroy(s_sts_list_node *node);

s_sts_list_node *sts_sdsnode_offset_node(s_sts_list_node *node_, int offset);
s_sts_list_node *sts_sdsnode_last_node(s_sts_list_node *node_);
s_sts_list_node *sts_sdsnode_first_node(s_sts_list_node *node_);
s_sts_list_node *sts_sdsnode_push(s_sts_list_node *node_, const void *in, size_t inlen);
s_sts_list_node *sts_sdsnode_update(s_sts_list_node *node_, const void *in, size_t inlen);
s_sts_list_node *sts_sdsnode_clone(s_sts_list_node *node_);
int sts_sdsnode_get_size(s_sts_list_node *node_);
int sts_sdsnode_get_count(s_sts_list_node *node_);

//------------------------sdsnode ------------------------------//
typedef struct s_sts_message_node {
	s_sts_sds	command;   //��Դ��Ϣר��,��ǰ��Ϣ��key  stsdb.get 
	s_sts_sds	key;       //��Դ��Ϣר��,��ǰ��Ϣ��key  sh600600.day 
	s_sts_sds	argv;      //��Դ��Ϣ�Ĳ�����Ϊjson��ʽ
	s_sts_sds	address;   //��Դ��Ϣר��, ������Դ��Ϣ����Ҫԭ�����أ��û�д��Ͷ�ݵ�ַ

	s_sts_list_node   *links;     //��Դ��Ϣר��, ������Դ��·��ÿ�ζ�һ��������һ���ڵ�
	s_sts_list_node   *nodes;     //��������Ϣ��������  node->value Ϊ s_sts_sds ����

} s_sts_message_node;


#define STS_MSN_ONE_VAL(a) (a->nodes ? (s_sts_sds)(a->nodes->value) : NULL)
#define STS_MSN_ONE_LEN(a) (a->nodes ? sts_sdslen((s_sts_sds)(a->nodes->value)) : 0)

////////////////////////////////////////////////////////
//  ���е��̺߳���������ݽ���ͳͳ�������ʽ����Ϣ�ṹ
////////////////////////////////////////////////////////

s_sts_message_node *sts_message_node_create();
void sts_message_node_destroy(void *in_);

s_sts_message_node *sts_message_node_clone(s_sts_message_node *in_);

#endif
//_STS_NODE_H