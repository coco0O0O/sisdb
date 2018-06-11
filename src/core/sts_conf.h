
#ifndef _STS_CONF_H
#define _STS_CONF_H

// ���������ͷ���ͻ������������е�osĿ¼���ļ�ͷ��
#include <sts_core.h>
#include <sts_file.h>
#include <sts_json.h>

// Ҫ����Դ�json�ַ���ת��Ϊconf��ʽ������js�ͻ������úò�����ֱ�ӷ��ظ����������д���
// Ҫ��֧�� # ע��
// Ҫ��֧�� include xxxx.conf 
// ֻ��Ҫ֧�ֽ����Ͷ�ȡ���������Ҫ���޸�

#define STS_CONF_NOTE_SIGN '#'  
#define STS_CONF_INCLUDE "include"  

typedef struct s_sts_conf_handle
{
	const char * error;     // ָ�����ĵ�ַ �����ַΪ�ձ�ʾ�����������
	char   path[STS_PATH_LEN];  //������conf��·��
	struct s_sts_json_node *node;
} s_sts_conf_handle;         //ר���ṩ����json�Ŀ��ٽṹ��

//-------- output option function-------- //

s_sts_conf_handle *sts_conf_open(const char *fn_); // ���ļ��� ����ȡע��
void sts_conf_close(s_sts_conf_handle *handle_); // �رղ��ͷ�

s_sts_conf_handle *sts_conf_load(const char *content_, size_t len_);
// ���json�ĸ�ʽ���ݣ�����conf��ע��
#define sts_conf_to_json sts_json_output
#define sts_conf_to_json_zip sts_json_output_zip

#define sts_conf_get_int sts_json_get_int
#define sts_conf_get_double sts_json_get_double
#define sts_conf_get_str sts_json_get_str
#define sts_conf_get_bool sts_json_get_bool

#define sts_conf_get_size sts_json_get_size
#define sts_conf_first_node sts_json_first_node
#define sts_conf_next_node sts_json_next_node
#define sts_conf_last_node sts_json_last_node

#define sts_conf_cmp_child_node sts_json_cmp_child_node
#define sts_conf_find_node  sts_json_find_node


#endif
