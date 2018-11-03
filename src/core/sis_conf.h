
#ifndef _SIS_CONF_H
#define _SIS_CONF_H

// ���������ͷ���ͻ������������е�osĿ¼���ļ�ͷ��
#include <sis_core.h>
#include <sis_file.h>
#include <sis_json.h>

// Ҫ����Դ�json�ַ���ת��Ϊconf��ʽ������js�ͻ������úò�����ֱ�ӷ��ظ����������д���
// Ҫ��֧�� # ע��
// Ҫ��֧�� include xxxx.conf 
// ֻ��Ҫ֧�ֽ����Ͷ�ȡ���������Ҫ���޸�

#define SIS_CONF_NOTE_SIGN '#'  
#define SIS_CONF_INCLUDE "include"  

typedef struct s_sis_conf_handle
{
	const char * error;     // ָ�����ĵ�ַ �����ַΪ�ձ�ʾ�����������
	char   path[SIS_PATH_LEN];  //������conf��·��
	struct s_sis_json_node *node;
} s_sis_conf_handle;         //ר���ṩ����json�Ŀ��ٽṹ��

//-------- output option function-------- //

s_sis_conf_handle *sis_conf_open(const char *fn_); // ���ļ��� ����ȡע��
void sis_conf_close(s_sis_conf_handle *handle_); // �رղ��ͷ�

s_sis_conf_handle *sis_conf_load(const char *content_, size_t len_);
// ���json�ĸ�ʽ���ݣ�����conf��ע��
#define sis_conf_to_json sis_json_output
#define sis_conf_to_json_zip sis_json_output_zip

#define sis_conf_get_int sis_json_get_int
#define sis_conf_get_double sis_json_get_double
#define sis_conf_get_str sis_json_get_str
#define sis_conf_get_bool sis_json_get_bool

#define sis_conf_get_size sis_json_get_size
#define sis_conf_first_node sis_json_first_node
#define sis_conf_next_node sis_json_next_node
#define sis_conf_last_node sis_json_last_node

#define sis_conf_cmp_child_node sis_json_cmp_child_node
#define sis_conf_find_node  sis_json_find_node


#endif
