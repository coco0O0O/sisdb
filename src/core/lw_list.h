#ifndef _LW_LIST_H
#define _LW_LIST_H

#include "lw_base.h"
#include "lw_string.h"
#include "adlist.h"
#include "sds.h"

// ����һ���ṹ������
// ����һ��ָ���д����
// ����һ���ַ�����д����

///////////////////////////////////////////////////////////////////////////
//------------------------s_struct_list ---------------------------------//
//  �̶����ȵ��б�
//////////////////////////////////////////////////////////////////////////
#define STRUCT_LIST_NORMAL   0
#define STRUCT_LIST_POINTER  1

typedef struct s_struct_list {
	int		     maxcount; //����
	int		     count;    //��ǰ����
	int          len;      //ÿ����¼�ĳ���
	int          mode;     //����ʲô���͵�����
	void        *buffer;   //������mallco�����char*����
	void(*free)(void *);   //==NULL ���ͷŶ�Ӧ�ڴ�
} s_struct_list;

#define BUFFLIST_STEP_ROW 128  //Ĭ�����ӵļ�¼��

s_struct_list *create_struct_list(int len_, void *in_, int inlen_); //len_Ϊÿ����¼����
void destroy_struct_list(s_struct_list *list_);
void clear_struct_list(s_struct_list *list_);
void struct_list_setsize(s_struct_list *list_, int len_);

int struct_list_push(s_struct_list *, void *in_);
int struct_list_insert(s_struct_list *, int index_, void *in_);
int struct_list_update(s_struct_list *, int index_, void *in_);
void *struct_list_get(s_struct_list *, int index_);
void *struct_list_next(s_struct_list *list_, void *, int offset);

int struct_list_set(s_struct_list *, void *in_, int inlen_);

void struct_list_limit(s_struct_list *, int limit_);
int struct_list_clone(s_struct_list *src_, s_struct_list *dst_, int limit_);
int struct_list_delete(s_struct_list *src_, int start_, int count_);
int struct_list_pack(s_struct_list *list_);

int struct_list_pto_recno(s_struct_list *list_,void *);

///////////////////////////////////////////////////////////////////////////
//------------------------s_pointer_list --------------------------------//
//  �洢ָ����б�,������struct_list,��¼����Ϊsizeof(char *)
///////////////////////////////////////////////////////////////////////////
//!!!Ҫ����
s_struct_list *create_pointer_list(); 

#define destroy_pointer_list destroy_struct_list
#define clear_pointer_list clear_struct_list
#define pointer_list_setlen struct_list_setlen
#define pointer_list_push struct_list_push
#define pointer_list_update struct_list_update
#define pointer_list_get struct_list_get
#define pointer_list_insert struct_list_insert
#define pointer_list_delete struct_list_delete

int pointer_list_indexof(s_struct_list *list_, void *in_);
int pointer_list_find_and_update(s_struct_list *, void *finder_, void *in_);
int pointer_list_find_and_delete(s_struct_list *list_, void *finder_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_string_list --------------------------------//
//  �洢�������ַ������б�
///////////////////////////////////////////////////////////////////////////
#define STRING_LIST_RD  1
#define STRING_LIST_WR  2

typedef struct s_string_list {
	int    permissions;     //Ȩ��
	char*  m_ptr_r;         // �����ֻ���ַ���
	s_struct_list *strlist; //�洢ָ���б� --freeΪ��ֻ�� ��Ϊ�տ�д
} s_string_list;

s_string_list *create_string_list_r(); //ֻ��
s_string_list *create_string_list_w(); //��д
void destroy_string_list(s_string_list *list_);
void clear_string_list(s_string_list *list_);

// read & write
int string_list_load(s_string_list *list_, const char *in_, size_t inlen_, const char *sign);
const char *string_list_get(s_string_list *list_, int index_);
int string_list_getsize(s_string_list *list_);

int string_list_indexof(s_string_list *list_, const char *in_);
int string_list_indeofcase(s_string_list *list_, const char *in_);

int string_list_update(s_string_list *list_, int index_, const char *in_, size_t inlen);
//�Ƚ��ַ�����ַ���޸ģ��ַ����Ƚ�Ӧ��ʹ��string_list_indexof&string_list_update
int string_list_find_and_update(s_string_list *list_, char *finder_, const char *in_, size_t inlen);
int string_list_insert(s_string_list *list_, int index_, const char *in_, size_t inlen);
int string_list_push(s_string_list *list_, const char *in_, size_t inlen);

int string_list_delete(s_string_list *list_, int index_);
//�Ƚ��ַ�����ַ��ɾ�����ַ����Ƚ�Ӧ��ʹ��string_list_indexof&string_list_delete
int string_list_find_and_delete(s_string_list *list_, const char *finder_);


///////////////////////////////////////////////////////////////////////////
//------------------------listNode --------------------------------//
//  ����listNode�б�ĺ���
///////////////////////////////////////////////////////////////////////////
listNode *create_sdsnode(const void *in, size_t inlen);
void destroy_sdsnode(listNode *node);

listNode *sdsnode_offset_node(listNode *node_, int offset);
listNode *sdsnode_last_node(listNode *node_);
listNode *sdsnode_first_node(listNode *node_);
listNode *sdsnode_push(listNode *node_, const void *in, size_t inlen);
listNode *sdsnode_update(listNode *node_, const void *in, size_t inlen);
listNode *sdsnode_clone(listNode *node_);
int sdsnode_get_size(listNode *node_);
int sdsnode_get_count(listNode *node_);

#endif //_LW_LIST_H