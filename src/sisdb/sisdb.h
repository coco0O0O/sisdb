
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SISDB_H
#define _SISDB_H

#include "sis_core.h"
#include "sis_map.h"
#include "sis_malloc.h"
#include "sis_time.h"
#include "sis_list.h"
#include "sis_json.h"
#include "sis_thread.h"

#define SIS_MAXLEN_CODE  16
#define SIS_MAXLEN_TABLE 32
#define SIS_MAXLEN_KEY   (SIS_MAXLEN_CODE + SIS_MAXLEN_TABLE)

#define SIS_MAXLEN_NAME  64

#define SIS_MAXLEN_COMMAND 128
#define SIS_MAXLEN_STRING  128

// Ĭ�Ϸ�����󲻳���32K�����ݣ�����������
#define SISDB_MAXLEN_RETURN (8*1024)
// ��Ҫ����ʱ����󻺴����ߴ�
// ��ĳ��������Ϊ disk ʱ����ʾ��Ҫ�����ڴ����ϣ���limitΪ�������ڴ�ļ�¼����
// ������������С���������������������Ű�ǰ�벿��д�̣�Ȼ���ƶ��ڴ�����ָ�룻
// getʱ��Ĭ��ֻȡ�ڴ������ݣ����Ǵ����� source:disk ��ô�Ż�ȥ���������е����ݣ��ٶ��ܴ��̷����ٶ�����
#define SISDB_MAXLEN_DISK_BUFFER (1024*1024)

#pragma pack(push, 1)

typedef struct s_sis_db {
	s_sis_sds name;            // ���ݿ����� sisdb

	s_sis_json_node    *conf;         // ��ǰ���ݱ��������Ϣ ����ʱ��

	s_sis_map_pointer  *dbs;          // ���ݱ���ֵ�� s_sisdb_table ����Ϊ���ݱ���

	bool   special;  // �Ƿ�֤ȯר�ã� ������system��Ķ��壬���Ҳ���ɾ���͸���
					 // һ���ᶨ���˿�����ʱ�䣬���ҿ���ʹ��PRICE�Լ�VOLUME��AMOUNT�ֶ�����
	s_sis_map_pointer  *sys_exchs;    // �г�����Ϣ һ���г�һ����¼ Ĭ�ϼ�¼Ϊ ��00��  s_sisdb_sys_exch
	s_sis_pointer_list *sys_infos;    // ��Ʊ����Ϣ ʵ�ʵ�info���� ��һ��ΪĬ�����ã���ͬ������ s_sisdb_sys_info
	s_sis_plan_task    *init_task;    // ��ʼ������

	s_sis_map_pointer  *collects;     // ���ݼ��ϵ��ֵ�� s_sisdb_collect ����ʵ�ʴ�����ݣ�����Ϊ��Ʊ����x���ݱ���

	s_sis_map_pointer  *map;          // �ؼ��ֲ�ѯ��
	
	s_sis_map_pointer  *methods;      // ���ݿ������������

	bool   loading;  // ���ݼ�����

	int 				save_format;   // �����ļ��ķ�ʽ
	s_sis_plan_task    *save_task;

}s_sis_db;

#pragma pack(pop)


s_sis_db *sisdb_create(char *);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sisdb_destroy(s_sis_db *);  //�ر�һ�����ݿ�

int sis_from_node_get_format(s_sis_db *db_, s_sis_json_node *node_);


#endif  /* _SISDB_H */
