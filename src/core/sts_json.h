
#ifndef _STS_JSON_H
#define _STS_JSON_H

#include <sts_core.h>
#include <stdarg.h>

#define STS_JSON_NULL    0
#define STS_JSON_ROOT    1 // ���ڵ�
#define STS_JSON_INT     2
#define STS_JSON_DOUBLE  3
#define STS_JSON_STRING  4
#define STS_JSON_ARRAY   5
#define STS_JSON_OBJECT  6

typedef struct s_sts_json_node
{
	struct s_sts_json_node *next, *prev; 
	struct s_sts_json_node *child;

	uint8  type;    // ��includeʱ���������ļ����������ݲ���content�м�������
	char  *key;			
	char  *value;
} s_sts_json_node;


typedef struct s_sts_json_handle
{
	const char * error;      // ָ�����ĵ�ַ
	bool   readonly;   // ��дģʽ��ֻ���ʹ�content
	char  *content;    // �ļ����ݣ�ֻ��ģʽ��ʹ�õ�ӳ��
	size_t position;   // ��ǰλ�ã���ʱ������ֻ������ʱ�� 
	struct s_sts_json_node *node;
} s_sts_json_handle; //ר���ṩ����json�Ŀ��ٽṹ��

//////////////////////////////////////////////
//   output main function define
///////////////////////////////////////////////
s_sts_json_handle *sts_json_open(const char *fn_);
void sts_json_delete_node(s_sts_json_handle *handle_, s_sts_json_node *node_);
void sts_json_close(s_sts_json_handle *handle_);

s_sts_json_handle *sts_json_load(const char *content_, size_t len_);  //�����ݼ���
void sts_json_save(s_sts_json_node *node_, const char *fn_); //��json�浽�ļ���

s_sts_json_node *sts_json_clone(s_sts_json_node *src_, int child_); // child_==0 ��ʾֻ��ǰ�ڵ�

//////////////////////////////////////////////
//======== write option =============//
//   write function define
///////////////////////////////////////////////
s_sts_json_node *sts_json_create_array();  //����һ������
s_sts_json_node *sts_json_create_object(); //����һ������

//*********************************//
//  ���º�������ı�ֻ������
//*********************************//

// ׷�ӵ�child��ĩβ��key������'.'
void sts_json_add_node(s_sts_json_handle *h_, s_sts_json_node *source_, s_sts_json_node *node_);

// ׷�ӵ�child��ĩβ��key������'.'
#define sts_json_object_add_node sts_json_add_node
// ���ӵ�child����ĩβ����һ��Ԫ��
// �޸ĵ�child�����û�з���key_,������һ��
void sts_json_object_add_int(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, long value_);
void sts_json_object_set_int(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, long value_);

void sts_json_object_add_double(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, double value_, int demical_);
void sts_json_object_set_double(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, double value_, int demical_);

void sts_json_object_add_string(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, const char *value_);
void sts_json_object_set_string(s_sts_json_handle *h_, s_sts_json_node *node_, const char *key_, const char *value_);

//----------------------------------------------//
// ׷�ӵ�child��ĩβ��key������'.'
#define sts_json_array_add_node sts_json_add_node
// ���ӵ�child����ĩβ����һ��Ԫ��
// �޸ĵ�child�����û�з���key_,������һ�� 
void sts_json_array_add_int(s_sts_json_handle *h_, s_sts_json_node *node_, long value_);
void sts_json_array_set_int(s_sts_json_handle *h_, s_sts_json_node *node_, int index_, long value_);

void sts_json_array_add_double(s_sts_json_handle *h_, s_sts_json_node *node_, double value_, int demical_);
void sts_json_array_set_double(s_sts_json_handle *h_, s_sts_json_node *node_, int index_,  double value_, int demical_);

void sts_json_array_add_string(s_sts_json_handle *h_, s_sts_json_node *node_, const char *value_);
void sts_json_array_set_string(s_sts_json_handle *h_, s_sts_json_node *node_, int index_,  const char *value_);

//-------------------------------------------------------------//
//======== read & print option =============//
//  �ṩ��������s_sts_json_nodeΪ�������ļ�read������Ĳ���
//-------------------------------------------------------------//
s_sts_json_node *sts_json_create_node(void);

char *sts_json_output(s_sts_json_node *node_, size_t *len_);
char *sts_json_output_zip(s_sts_json_node *node_, size_t *len_);

//======== read option =============//

int sts_json_get_int(s_sts_json_node *root_, const char *key_, int defaultvalue_);
double sts_json_get_double(s_sts_json_node *root_, const char *key_, double defaultvalue_);
const char *sts_json_get_str(s_sts_json_node *root_, const char *key_);
bool sts_json_get_bool(s_sts_json_node *root_, const char *key_, bool defaultvalue_);

int sts_json_get_size(s_sts_json_node *node_);
s_sts_json_node *sts_json_first_node(s_sts_json_node *node_);
s_sts_json_node *sts_json_next_node(s_sts_json_node *node_);
s_sts_json_node *sts_json_last_node(s_sts_json_node *node_);
//����·����ȡ���ݣ�·��Ϊ aaa.bbb.ccc
s_sts_json_node *sts_json_cmp_child_node(s_sts_json_node *object_, const char *key_);
s_sts_json_node *sts_json_find_node(s_sts_json_node *node_, const char *path_);


#endif
