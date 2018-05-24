#ifndef _STS_LIST_H
#define _STS_LIST_H

#include "sts_core.h"
#include "sts_str.h"
#include "adlist.h"
#include "sds.h"
#include "zmalloc.h"
// ����һ���ṹ������
// ����һ��ָ���д����
// ����һ���ַ�����д����

///////////////////////////////////////////////////////////////////////////
//------------------------s_sts_struct_list ---------------------------------//
//  �̶����ȵ��б�
//////////////////////////////////////////////////////////////////////////
#define STRUCT_LIST_NORMAL   0
#define STRUCT_LIST_POINTER  1

typedef struct s_sts_struct_list {
	int		     maxcount; //����
	int		     count;    //��ǰ����
	int          len;      //ÿ����¼�ĳ���
	int          mode;     //����ʲô���͵�����
	void        *buffer;   //������mallco�����char*����
	void(*free)(void *);   //==NULL ���ͷŶ�Ӧ�ڴ�
} s_sts_struct_list;

#define BUFFLIST_STEP_ROW 128  //Ĭ�����ӵļ�¼��

s_sts_struct_list *sts_struct_list_create(int len_, void *in_, int inlen_); //len_Ϊÿ����¼����
void sts_struct_list_destroy(s_sts_struct_list *list_);
void sts_struct_list_clear(s_sts_struct_list *list_);
void struct_list_setsize(s_sts_struct_list *list_, int len_);

int sts_struct_list_push(s_sts_struct_list *, void *in_);
int sts_struct_list_insert(s_sts_struct_list *, int index_, void *in_);
int sts_struct_list_update(s_sts_struct_list *, int index_, void *in_);
void *sts_struct_list_get(s_sts_struct_list *, int index_);
void *sts_struct_list_next(s_sts_struct_list *list_, void *, int offset);

int sts_struct_list_set(s_sts_struct_list *, void *in_, int inlen_);

void sts_struct_list_limit(s_sts_struct_list *, int limit_);
int sts_struct_list_clone(s_sts_struct_list *src_, s_sts_struct_list *dst_, int limit_);
int sts_struct_list_delete(s_sts_struct_list *src_, int start_, int count_);
int sts_struct_list_pack(s_sts_struct_list *list_);

// ��ȡָ���λ�ñ��
int sts_struct_list_pto_recno(s_sts_struct_list *list_,void *);

///////////////////////////////////////////////////////////////////////////
//------------------------s_pointer_list --------------------------------//
//  �洢ָ����б�,������struct_list,��¼����Ϊsizeof(char *)
///////////////////////////////////////////////////////////////////////////
//!!!Ҫ����
s_sts_struct_list *sts_pointer_list_create(); 

#define sts_pointer_list_destroy sts_struct_list_destroy
#define sts_pointer_list_clear sts_struct_list_clear
#define sts_pointer_list_setlen struct_list_setlen
#define sts_pointer_list_push sts_struct_list_push
#define sts_pointer_list_update sts_struct_list_update
#define sts_pointer_list_get sts_struct_list_get
#define sts_pointer_list_insert sts_struct_list_insert
#define sts_pointer_list_delete sts_struct_list_delete

int sts_pointer_list_indexof(s_sts_struct_list *list_, void *in_);
int sts_pointer_list_find_and_update(s_sts_struct_list *, void *finder_, void *in_);
int sts_pointer_list_find_and_delete(s_sts_struct_list *list_, void *finder_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_sts_string_list --------------------------------//
//  �洢�������ַ������б�
///////////////////////////////////////////////////////////////////////////
#define STRING_LIST_RD  1
#define STRING_LIST_WR  2

typedef struct s_sts_string_list {
	int    permissions;     //Ȩ��
	char*  m_ptr_r;         // �����ֻ���ַ���
	s_sts_struct_list *strlist; //�洢ָ���б� --freeΪ��ֻ�� ��Ϊ�տ�д
} s_sts_string_list;

s_sts_string_list *sts_string_list_create_r(); //ֻ��
s_sts_string_list *sts_string_list_create_w(); //��д
void sts_string_list_destroy(s_sts_string_list *list_);
void sts_string_list_clear(s_sts_string_list *list_);

// read & write
int sts_string_list_load(s_sts_string_list *list_, const char *in_, size_t inlen_, const char *sign);
const char *sts_string_list_get(s_sts_string_list *list_, int index_);
int sts_string_list_getsize(s_sts_string_list *list_);

int sts_string_list_indexof(s_sts_string_list *list_, const char *in_);
int sts_string_list_indexofcase(s_sts_string_list *list_, const char *in_);

int sts_string_list_update(s_sts_string_list *list_, int index_, const char *in_, size_t inlen);
//�Ƚ��ַ�����ַ���޸ģ��ַ����Ƚ�Ӧ��ʹ��string_list_indexof&sts_string_list_update
int sts_string_list_find_and_update(s_sts_string_list *list_, char *finder_, const char *in_, size_t inlen);
int sts_string_list_insert(s_sts_string_list *list_, int index_, const char *in_, size_t inlen);
int sts_string_list_push(s_sts_string_list *list_, const char *in_, size_t inlen);

int sts_string_list_delete(s_sts_string_list *list_, int index_);
//�Ƚ��ַ�����ַ��ɾ�����ַ����Ƚ�Ӧ��ʹ��string_list_indexof&sts_string_list_delete
int sts_string_list_find_and_delete(s_sts_string_list *list_, const char *finder_);


///////////////////////////////////////////////////////////////////////////
//------------------------listNode --------------------------------//
//  ����listNode�б�ĺ���
///////////////////////////////////////////////////////////////////////////
listNode *sts_sdsnode_create(const void *in, size_t inlen);
void sts_sdsnode_destroy(listNode *node);

listNode *sts_sdsnode_offset_node(listNode *node_, int offset);
listNode *sts_sdsnode_last_node(listNode *node_);
listNode *sts_sdsnode_first_node(listNode *node_);
listNode *sts_sdsnode_push(listNode *node_, const void *in, size_t inlen);
listNode *sts_sdsnode_update(listNode *node_, const void *in, size_t inlen);
listNode *sts_sdsnode_clone(listNode *node_);
int sts_sdsnode_get_size(listNode *node_);
int sts_sdsnode_get_count(listNode *node_);

#endif //_STS_LIST_H