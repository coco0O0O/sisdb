
#ifndef _LW_JSON_H
#define _LW_JSON_H

#include "lw_base.h"

#define DP_JSON_NULL   0
//#define DP_JSON_BOOL   1
#define DP_JSON_INT    2
#define DP_JSON_DOUBLE 3
#define DP_JSON_STRING 4
#define DP_JSON_ARRAY  5
#define DP_JSON_OBJECT 6

typedef struct s_json_node
{
	struct s_json_node *next, *prev; /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct s_json_node *child;		 /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */
	int type;						 /* The type of the item, as above. */
	char *key_w;					 /* The item's name,  ֻ��״̬ʱ���˴�Ϊ0��������������ʱ��ͬʱ����key_r*/
	char *value_w;					 /* The item's value, ֻ��״̬ʱ���˴�Ϊ0��������������ʱ��ͬʱ����value_r*/
	char *key_r;					 /* The item's name,  ��ȡ����ʱ��ֻ�����Ĭ�ϼ���ʱ���������content��mapӳ���ַ�������޸ķ���ʱ�������key_w��ַһ��*/
	char *value_r;					 /* The item's value, */
} s_json_node;

typedef struct dp_json_read_handle
{
	char *content; //�ļ����ݣ�����ӳ��
	int position;
	struct s_json_node *node;
} dp_json_read_handle; //ר���ṩ����json�Ŀ��ٽṹ��

//-------- output option function-------- //
#ifdef __cplusplus
extern "C" {
#endif

dp_json_read_handle *dp_json_open(const char *filename_);
void dp_json_close(dp_json_read_handle *handle_);
void dp_json_free(s_json_node *c);

dp_json_read_handle *dp_json_load(const char *content_);  //�����ݼ���
void dp_json_save(s_json_node *node_, const char *outfilename_); //��json�浽�ļ���

char *dp_json_print(s_json_node *item);
char *dp_json_print_unformatted(s_json_node *item);
/* clone a s_json_node item */
s_json_node *dp_json_clone(s_json_node *src_, int recurse);

//-------------------------------------------------------------//

int dp_json_get_int(s_json_node *root, const char *key_, int defaultvalue);
double dp_json_get_double(s_json_node *root, const char *key_, double defaultvalue);
const char *dp_json_get_str(s_json_node *root, const char *key_);
bool dp_json_get_bool(s_json_node *root, const char *key_, bool defaultvalue);

int dp_json_get_array_int(s_json_node *root, int index_, int defaultvalue);
double dp_json_get_array_double(s_json_node *root, int index_, double defaultvalue);
const char *dp_json_get_array_str(s_json_node *root, int index_);
bool dp_json_get_array_bool(s_json_node *root, int index_, bool defaultvalue);

int dp_json_array_get_size(s_json_node *array_);
s_json_node *dp_json_array_get_item(s_json_node *array_, int index_);
s_json_node *dp_json_object_get_item(s_json_node *object_, const char *key_);
s_json_node *dp_json_next_item(s_json_node *now_);
//����·����ȡ���ݣ�
s_json_node *dp_json_find_item(s_json_node *node, const char *path_);

//======== write option =============//
s_json_node *dp_json_create_array();  //����һ������
s_json_node *dp_json_create_object(); //����һ������
void dp_json_free(s_json_node *c);

//if key_==NULL,��ʾ�½�һ��json "{}"
void dp_json_object_add_item(s_json_node *object, const char *key_, s_json_node *item);
//if key==NULL,��ʾ�½�һ��array "[]"
void dp_json_array_add_item(s_json_node *array_, s_json_node *item);

void dp_json_array_add_int(s_json_node *node_, long value_);
void dp_json_array_add_double(s_json_node *node_, double value_);
void dp_json_array_add_string(s_json_node *node_, const char *value_);

void dp_json_set_int(s_json_node *node_, const char *key_, long value_);
void dp_json_set_double(s_json_node *node_, const char *key_, double value_, int demical_);
void dp_json_set_string(s_json_node *node_, const char *key_, const char *value_);

//���key_Ϊ�գ���ֱ���޸�node��ֵ
void dp_json_array_delete_item(s_json_node *array_, int which);
s_json_node *dp_json_object_detach_item(s_json_node *object, const char *key_);
void dp_json_object_delete_item(s_json_node *object, const char *key_);
void dp_json_array_set_item(s_json_node *array_, int index_, s_json_node *newitem);
void dp_json_object_set_item(s_json_node *src_, const char *key_, s_json_node *newitem);

#ifdef __cplusplus
}
#endif

#endif
