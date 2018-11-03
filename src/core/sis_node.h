#ifndef _SIS_NODE_H
#define _SIS_NODE_H

#include "sis_core.h"
#include "sis_malloc.h"

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_list_node --------------------------------//
//  ���� s_sis_list_node �б�ĺ���
//  value Ϊ s_sis_sds ����
///////////////////////////////////////////////////////////////////////////
s_sis_list_node *sis_sdsnode_create(const void *in, size_t inlen);
void sis_sdsnode_destroy(s_sis_list_node *node);

s_sis_list_node *sis_sdsnode_offset_node(s_sis_list_node *node_, int offset);
s_sis_list_node *sis_sdsnode_last_node(s_sis_list_node *node_);
s_sis_list_node *sis_sdsnode_first_node(s_sis_list_node *node_);
s_sis_list_node *sis_sdsnode_next_node(s_sis_list_node *node_);
s_sis_list_node *sis_sdsnode_push(s_sis_list_node *node_, const void *in, size_t inlen);
s_sis_list_node *sis_sdsnode_update(s_sis_list_node *node_, const void *in, size_t inlen);
s_sis_list_node *sis_sdsnode_clone(s_sis_list_node *node_);
int sis_sdsnode_get_size(s_sis_list_node *node_);
int sis_sdsnode_get_count(s_sis_list_node *node_);

//------------------------sdsnode ------------------------------//
typedef struct s_sis_message_node {
	s_sis_sds	command;   //��Դ��Ϣר��,��ǰ��Ϣ��key  sisdb.get 
	s_sis_sds	key;       //��Դ��Ϣר��,��ǰ��Ϣ��key  sh600600.day 
	s_sis_sds	argv;      //��Դ��Ϣ�Ĳ�����Ϊjson��ʽ
	s_sis_sds	address;   //��Դ��Ϣר��, ������Դ��Ϣ����Ҫԭ�����أ��û�д��Ͷ�ݵ�ַ

	s_sis_sds   format;    //set���͵����ݸ�ʽ struct json

	s_sis_list_node   *links;     //��Դ��Ϣר��, ������Դ��·��ÿ�ζ�һ��������һ���ڵ�
	s_sis_list_node   *nodes;     //��������Ϣ��������  node->value Ϊ s_sis_sds sis_

} s_sis_message_node;


#define SIS_MSN_ONE_VAL(a) (a->nodes ? (s_sis_sds)(a->nodes->value) : NULL)
#define SIS_MSN_ONE_LEN(a) (a->nodes ? sis_sdslen((s_sis_sds)(a->nodes->value)) : 0)

////////////////////////////////////////////////////////
//  ���е��̺߳���������ݽ���ͳͳ�������ʽ����Ϣ�ṹ
////////////////////////////////////////////////////////

s_sis_message_node *sis_message_node_create();
void sis_message_node_destroy(void *in_);

s_sis_message_node *sis_message_node_clone(s_sis_message_node *in_);

#endif
//_SIS_NODE_H