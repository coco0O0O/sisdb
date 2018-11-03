
#ifndef _SIS_JSON_H
#define _SIS_JSON_H

#include <sis_core.h>
#include <sis_file.h>
#include <sis_str.h>

#define SIS_JSON_NULL    0
#define SIS_JSON_ROOT    1 // ���ڵ�
#define SIS_JSON_INT     2
#define SIS_JSON_DOUBLE  3
#define SIS_JSON_STRING  4
#define SIS_JSON_ARRAY   5
#define SIS_JSON_OBJECT  6

typedef struct s_sis_json_node
{
	struct s_sis_json_node *next, *prev; 
	struct s_sis_json_node *child, *father;

	uint8  type;    // ��includeʱ���������ļ����������ݲ���content�м�������
	char  *key;			
	char  *value;
} s_sis_json_node;


typedef struct s_sis_json_handle
{
	const char * error;      // ָ�����ĵ�ַ
	// bool   readonly;   // ��дģʽ��ֻ���ʹ�content
	// char  *content;    // �ļ����ݣ�ֻ��ģʽ��ʹ�õ�ӳ��
	// size_t position;   // ��ǰλ�ã���ʱ������ֻ������ʱ�� 
	struct s_sis_json_node *node;
} s_sis_json_handle; //ר���ṩ����json�Ŀ��ٽṹ��

//////////////////////////////////////////////
//   output main function define
///////////////////////////////////////////////
s_sis_json_handle *sis_json_open(const char *fn_);
void sis_json_delete_node(s_sis_json_node *node_);
void sis_json_close(s_sis_json_handle *handle_);

s_sis_json_handle *sis_json_load(const char *content_, size_t len_);  //�����ݼ���
void sis_json_save(s_sis_json_node *node_, const char *fn_); //��json�浽�ļ���

s_sis_json_node *sis_json_clone(s_sis_json_node *src_, int child_); // child_==0 ��ʾֻ��ǰ�ڵ�

//////////////////////////////////////////////
//======== write option =============//
//   write function define
///////////////////////////////////////////////
s_sis_json_node *sis_json_create_array();  //����һ������
s_sis_json_node *sis_json_create_object(); //����һ������

//*********************************//
//  ���º�������ı�ֻ������
//*********************************//

// ׷�ӵ�child��ĩβ��key������'.'
void sis_json_array_add_node(s_sis_json_node *source_, s_sis_json_node *node_);
void sis_json_object_add_node(s_sis_json_node *source_, const char *key_, s_sis_json_node *node_);

// ׷�ӵ�child��ĩβ��key������'.'
// ���ӵ�child����ĩβ����һ��Ԫ��
// �޸ĵ�child�����û�з���key_,������һ��
void sis_json_object_add_int(s_sis_json_node *node_, const char *key_, long value_);
void sis_json_object_set_int(s_sis_json_node *node_, const char *key_, long value_);

void sis_json_object_add_uint(s_sis_json_node *node_, const char *key_, unsigned long value_);
void sis_json_object_set_uint(s_sis_json_node *node_, const char *key_, unsigned long value_);

void sis_json_object_add_double(s_sis_json_node *node_, const char *key_, double value_, int demical_);
void sis_json_object_set_double(s_sis_json_node *node_, const char *key_, double value_, int demical_);

void sis_json_object_add_string(s_sis_json_node *node_, const char *key_, const char *value_, size_t len_);
void sis_json_object_set_string(s_sis_json_node *node_, const char *key_, const char *value_, size_t len_);

//----------------------------------------------//
// ׷�ӵ�child��ĩβ��key������'.'
// ���ӵ�child����ĩβ����һ��Ԫ��
// �޸ĵ�child�����û�з���key_,������һ�� 
void sis_json_array_add_int(s_sis_json_node *node_, long value_);
void sis_json_array_set_int(s_sis_json_node *node_, int index_, long value_);

void sis_json_array_add_uint(s_sis_json_node *node_, unsigned long value_);
void sis_json_array_set_uint(s_sis_json_node *node_, int index_, unsigned long value_);

void sis_json_array_add_double(s_sis_json_node *node_, double value_, int demical_);
void sis_json_array_set_double(s_sis_json_node *node_, int index_,  double value_, int demical_);

void sis_json_array_add_string(s_sis_json_node *node_, const char *value_, size_t len_);
void sis_json_array_set_string(s_sis_json_node *node_, int index_,  const char *value_, size_t len_);

//-------------------------------------------------------------//
//======== read & print option =============//
//  �ṩ��������s_sis_sis__nodeΪ�������ļ�read������Ĳ���
//-------------------------------------------------------------//
s_sis_json_node *sis_json_create_node(void);

char *sis_json_output(s_sis_json_node *node_, size_t *len_);
char *sis_json_output_zip(s_sis_json_node *node_, size_t *len_);

void sis_json_printf(s_sis_json_node *node_, int *i);
//======== read option =============//

int64 sis_json_get_int(s_sis_json_node *root_, const char *key_, int64 defaultvalue_);
double sis_json_get_double(s_sis_json_node *root_, const char *key_, double defaultvalue_);
const char *sis_json_get_str(s_sis_json_node *root_, const char *key_);
bool sis_json_get_bool(s_sis_json_node *root_, const char *key_, bool defaultvalue_);

int64 sis_array_get_int(s_sis_json_node *root_, int index_, int64 defaultvalue_);
double sis_array_get_double(s_sis_json_node *root_, int index_, double defaultvalue_);
const char *sis_array_get_str(s_sis_json_node *root_, int index_);
bool sis_json_array_bool(s_sis_json_node *root_, int index_, bool defaultvalue_);

int sis_json_get_size(s_sis_json_node *node_);
s_sis_json_node *sis_json_first_node(s_sis_json_node *node_);
s_sis_json_node *sis_json_next_node(s_sis_json_node *node_);
s_sis_json_node *sis_json_last_node(s_sis_json_node *node_);
//����·����ȡ���ݣ�·��Ϊ aaa.bbb.ccc
s_sis_json_node *sis_json_cmp_child_node(s_sis_json_node *object_, const char *key_);
s_sis_json_node *sis_json_find_node(s_sis_json_node *node_, const char *path_);


#endif
