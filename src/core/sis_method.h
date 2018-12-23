
#ifndef _SIS_METHOD_H
#define _SIS_METHOD_H

#include <sis_core.h>
#include <sis_json.h>
#include "sis_map.h"
#include "sis_list.h"

// typedef int _sis_method_define(void *, s_sis_json_node *);
// typedef int _sis_method_logic_define(void *, void *);

typedef struct s_sis_method {
    const char *name;   // ����������
    const char *style;  // �������ڵ�����൱�������ռ� subscribe append zip ��
	void *(*proc)(void *, s_sis_json_node *);
    // _sis_method_define *proc;
	const char *explain;  // ������˵��
}s_sis_method;

// ����Ĭ��Ϊһ���ֶ�
typedef struct s_sis_method_node {
    s_sis_method    *method;
	s_sis_json_node *argv;
	void            *out;   // ����������ָ�룬������SIS_METHOD_CLASS_FILTER����ʱ���ɣ����ڽ�����������
	// int              option; // ���ϴν���Ƚ��� 0 �뻹�� 1��
    struct s_sis_method_node *next, *prev;    // ��Ĺ�ϵ
    struct s_sis_method_node *child, *father; // ��Ĺ�ϵ
}s_sis_method_node;

#define SIS_METHOD_CLASS_MARKING  0 // ������Դ���ǣ�ѡ����, �Դ�������������
#define SIS_METHOD_CLASS_FILTER   1 // �����in����ͬ���Ľ�����ݼ������ݿ��ѯʱ��
// ���뷽���Ľṹ����
// in��out������ͬ�������ݣ�outӦ����in��ͬ�����Ӽ�
// �����¼������
// 1.��������ǣ�out��inһ����merge��across����Ϊ�վͿ����ˣ������в������ݼ��ٶ���
// 2.ȡin���Ӽ���

typedef struct s_sis_method_class {
	int      	style;  // ����
	void       *obj;     // ���������ָ�룬
	s_sis_method_node        *node;   // ����������������value������
	void(*merge)(void *, void *);      // ��ͬһ�ȼ��ķ����󲢼�
	void(*across)(void *, void *);     // ����һ�ȼ��ķ����󽻼�
	void(*free)(void *);          // free�ķ���,�ͷ�in��out
	void*(*malloc)();       // malloc�ķ���,��������out����һ��inʱʹ��
	// _sis_method_logic_define *merge;  // ��ͬһ�ȼ��ķ����󲢼�
	// _sis_method_logic_define *across; // ����һ�ȼ��ķ����󽻼�
}s_sis_method_class;
//////////////////////////////////////////////
//   method_map ���ﶨ����Ƿ����б�
///////////////////////////////////////////////
s_sis_map_pointer *sis_method_map_create(s_sis_method *methods_, int count_);
s_sis_method *sis_method_map_find(s_sis_map_pointer *map_, const char *name_, const char *style_);
void sis_method_map_destroy(s_sis_map_pointer *map_);

//////////////////////////////////////////////
//   method_node ���ﶨ�����ʵ�ʷ����Ĳ���������
///////////////////////////////////////////////

// s_sis_method_node* _sis_method_node_load(
// 		s_sis_map_pointer *map_,
// 		s_sis_method_node *first_,
// 		s_sis_method_node *father_,
// 		const char *style_, 
// 		s_sis_json_node *node_);

// // ��㣬�����Ϊһ�����߹�ϵ�ķ����б�
// s_sis_method_node *sis_method_node_create(
// 	s_sis_map_pointer *map_,
// 	const char *style_, 
// 	s_sis_json_node *node_);

// void sis_method_node_destroy(void *node_, void *other_);
// 
s_sis_method_class *sis_method_class_create(
	s_sis_map_pointer *map_,
	const char *style_, 
	s_sis_json_node *node_);

void *sis_method_class_execute(s_sis_method_class *class_);

// 0 -  ȫ������  1 - ֻ�������������
void sis_method_class_clear(s_sis_method_class *,int);

void sis_method_class_destroy(void *node_, void *other_);

#endif
