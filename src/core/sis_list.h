#ifndef _SIS_LIST_H
#define _SIS_LIST_H

#include "sis_core.h"
#include "sis_str.h"

#include "sis_malloc.h"

// ����һ���ṹ������
// ����һ��ָ���д����
// ����һ���ַ�����д����

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_struct_list ---------------------------------//
//  �̶����ȵ��б�
//////////////////////////////////////////////////////////////////////////
#define STRUCT_LIST_NORMAL   0
#define STRUCT_LIST_POINTER  1

typedef struct s_sis_struct_list {
	int		     maxcount; // ����
	int		     count;    // ��ǰ����
	int          len;      // ÿ����¼�ĳ���
	int          mode;     // ����ʲô���͵�����
	void        *buffer;   // ������mallco�����char*����
	void(*free)(void *);   // ==NULL ���ͷŶ�Ӧ�ڴ�
} s_sis_struct_list;

#define BUFFLIST_STEP_ROW 128  //Ĭ�����ӵļ�¼��

s_sis_struct_list *sis_struct_list_create(int len_, void *in_, int inlen_); //len_Ϊÿ����¼����
void sis_struct_list_destroy(s_sis_struct_list *list_);
void sis_struct_list_clear(s_sis_struct_list *list_);
void struct_list_setsize(s_sis_struct_list *list_, int len_);

int sis_struct_list_push(s_sis_struct_list *, void *in_);
int sis_struct_list_insert(s_sis_struct_list *, int index_, void *in_);
int sis_struct_list_update(s_sis_struct_list *, int index_, void *in_);
void *sis_struct_list_first(s_sis_struct_list *);
void *sis_struct_list_last(s_sis_struct_list *);
void *sis_struct_list_get(s_sis_struct_list *, int index_);
void *sis_struct_list_next(s_sis_struct_list *list_, void *, int offset);

int sis_struct_list_set(s_sis_struct_list *, void *in_, int inlen_);
int sis_struct_list_setlen(s_sis_struct_list *, int count_);

void sis_struct_list_limit(s_sis_struct_list *, int limit_);
int sis_struct_list_clone(s_sis_struct_list *src_, s_sis_struct_list *dst_, int limit_);
int sis_struct_list_delete(s_sis_struct_list *src_, int start_, int count_);
int sis_struct_list_pack(s_sis_struct_list *list_);

// ��ȡָ���λ�ñ��
int sis_struct_list_pto_recno(s_sis_struct_list *list_,void *);

///////////////////////////////////////////////////////////////////////////
//------------------------s_pointer_list --------------------------------//
//  �洢ָ����б�,������struct_list,��¼����Ϊsizeof(char *)
///////////////////////////////////////////////////////////////////////////
//
#define s_sis_pointer_list s_sis_struct_list

s_sis_struct_list *sis_pointer_list_create(); 

#define sis_pointer_list_destroy sis_struct_list_destroy
#define sis_pointer_list_clear sis_struct_list_clear
#define sis_pointer_list_setlen struct_list_setlen
#define sis_pointer_list_push sis_struct_list_push
#define sis_pointer_list_update sis_struct_list_update
#define sis_pointer_list_get sis_struct_list_get
#define sis_pointer_list_insert sis_struct_list_insert
#define sis_pointer_list_delete sis_struct_list_delete

int sis_pointer_list_indexof(s_sis_struct_list *list_, void *in_);
int sis_pointer_list_find_and_update(s_sis_struct_list *, void *finder_, void *in_);
int sis_pointer_list_find_and_delete(s_sis_struct_list *list_, void *finder_);

///////////////////////////////////////////////////////////////////////////
//------------------------s_sis_string_list --------------------------------//
//  �洢�������ַ������б�
///////////////////////////////////////////////////////////////////////////
#define STRING_LIST_RD  1
#define STRING_LIST_WR  2
void sis_free_call(void *p);

typedef struct s_sis_string_list {
	int    permissions;     //Ȩ��
	char*  m_ptr_r;         // �����ֻ���ַ���
	s_sis_struct_list *strlist; //�洢ָ���б� --freeΪ��ֻ�� ��Ϊ�տ�д
} s_sis_string_list;

s_sis_string_list *sis_string_list_create_r(); //ֻ��
s_sis_string_list *sis_string_list_create_w(); //��д
void sis_string_list_destroy(void *list_);
void sis_string_list_clear(s_sis_string_list *list_);

// read & write
int sis_string_list_load(s_sis_string_list *list_, const char *in_, size_t inlen_, const char *sign);
const char *sis_string_list_get(s_sis_string_list *list_, int index_);
int sis_string_list_getsize(s_sis_string_list *list_);

s_sis_sds sis_string_list_sds(s_sis_string_list *list_);

int sis_string_list_clone(
	s_sis_string_list *src_, 
	s_sis_string_list *des_);
int sis_string_list_merge(
	s_sis_string_list *list_, 
	s_sis_string_list *other_); 
int sis_string_list_across(
	s_sis_string_list *list_, 
	s_sis_string_list *other_);

int sis_string_list_indexof(s_sis_string_list *list_, const char *in_);
int sis_string_list_indexofcase(s_sis_string_list *list_, const char *in_);

int sis_string_list_update(s_sis_string_list *list_, int index_, const char *in_, size_t inlen);
//�Ƚ��ַ�����ַ���޸ģ��ַ����Ƚ�Ӧ��ʹ��string_list_indexof&sis_string_sis__update
int sis_string_list_find_and_update(s_sis_string_list *list_, char *finder_, const char *in_, size_t inlen);
int sis_string_list_insert(s_sis_string_list *list_, int index_, const char *in_, size_t inlen);
int sis_string_list_push(s_sis_string_list *list_, const char *in_, size_t inlen);
int sis_string_list_push_only(s_sis_string_list *list_, const char *in_, size_t inlen);

void sis_string_list_limit(s_sis_string_list *list_, int limit_);

int sis_string_list_delete(s_sis_string_list *list_, int index_);
//�Ƚ��ַ�����ַ��ɾ�����ַ����Ƚ�Ӧ��ʹ��string_list_indexof&sis_string_lsis_delete
int sis_string_list_find_and_delete(s_sis_string_list *list_, const char *finder_);



#endif //_SIS_LIST_H