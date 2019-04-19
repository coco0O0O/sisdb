
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

// һЩ��Ҫ�ĳ�������
#define SIS_MAXLEN_CODE  16  // ���볤��
#define SIS_MAXLEN_TABLE 32  // ���ݱ�������
#define SIS_MAXLEN_KEY   (SIS_MAXLEN_CODE + SIS_MAXLEN_TABLE)

#define SIS_MAXLEN_NAME  64  // ����������

#define SIS_MAXLEN_COMMAND 128   // �����
#define SIS_MAXLEN_STRING  128   // ���ʱ�����ַ�ͨ�ó���

// Ĭ�Ϸ�����󲻳���16K�����ݣ�����������
#define SISDB_MAXLEN_RETURN (16*1024)
// ��Ҫ����ʱ����󻺴����ߴ�
// ��ĳ��������Ϊ disk ʱ����ʾ��Ҫ�����ڴ����ϣ���limitΪ�������ڴ�ļ�¼����
// ������������С���������������������Ű�ǰ�벿��д�̣�Ȼ���ƶ��ڴ�����ָ�룻
// getʱ��Ĭ��ֻȡ�ڴ������ݣ����Ǵ����� source:disk ��ô�Ż�ȥ���������е����ݣ��ٶ��ܴ��̷����ٶ�����
#define SISDB_MAXLEN_DISK_BUFFER (16*1024*1024)

#pragma pack(push, 1)

typedef struct s_sis_db {
	s_sis_sds name;            // ���ݿ����� sisdb

	s_sis_json_node    *conf;         // ��ǰ���ݱ��������Ϣ ����ʱ��

	s_sis_sds  value;    // ȱʡ�ı������µļ�¼����Ϊ�գ�������������¼Ϊģ��
	
	bool   loading;  // ���ݼ�����
	
	s_sis_mutex_t  write_mutex;        // ��ǰ���ݼ���д��ʱ����

	s_sis_map_pointer  *dbs;          // ���ݱ���ֵ�� s_sisdb_table ����Ϊ���ݱ���

	s_sis_map_pointer  *collects;     // ���ݼ��ϵ��ֵ�� s_sisdb_collect ����ʵ�ʴ�����ݣ�����Ϊ��Ʊ����x���ݱ���

	s_sis_pointer_list *subscribes;   // �������б� �������и��ص�����

	int 				save_format;   // �����ļ��ķ�ʽ

	// bool   special;  // �Ƿ�֤ȯר�ã� ������system��Ķ��壬���Ҳ���ɾ���͸���
	// 				 // һ���ᶨ���˿�����ʱ�䣬���ҿ���ʹ��PRICE�Լ�VOLUME��AMOUNT�ֶ�����
	// s_sis_map_pointer  *sys_exchs;    // �г�����Ϣ һ���г�һ����¼ Ĭ�ϼ�¼Ϊ ��00��  s_sisdb_sys_exch
	// s_sis_pointer_list *sys_infos;    // ��Ʊ����Ϣ ʵ�ʵ�info���� ��һ��ΪĬ�����ã���ͬ������ s_sisdb_sys_info
	// s_sis_plan_task    *init_task;    // ��ʼ������

	// s_sis_plan_task    *save_task;
	sis_file_handle    aof_fp;
}s_sis_db;


typedef struct s_sisdb_worker {
	int    status;         //��ǰ�Ĺ���״̬

    char   classname[SIS_NAME_LEN];
    
    void   *other;   // �����������
    void   *father;  // server��ָ�룬����ѭ���˳�ʹ��

    s_sis_plan_task    *task;  // ��Ӧ������ ��������������

	bool(*open)(struct s_sisdb_worker *, s_sis_json_node *);
    void(*working)(struct s_sisdb_worker *);
	void(*close)(struct s_sisdb_worker *);

} s_sisdb_worker;


typedef struct s_sisdb_save_config {
	int    status;         //��ǰ�Ĺ���״̬ 
} s_sisdb_save_config;

typedef struct s_sisdb_init_config {
	char dbname[128];
	char tbname[128];
} s_sisdb_init_config;

#pragma pack(pop)

s_sis_db *sisdb_create(const char *);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sisdb_destroy(s_sis_db *);  //�ر�һ�����ݿ�

s_sisdb_worker *sisdb_worker_create(s_sis_json_node *);
void sisdb_worker_destroy(s_sisdb_worker *);
// ��ȡ���������ú���
void sisdb_worker_init(s_sisdb_worker *, s_sis_json_node *);

///////////////////////////
//  save worker define   //
///////////////////////////

bool worker_save_open(s_sisdb_worker *, s_sis_json_node *);
void worker_save_close(s_sisdb_worker *);
void worker_save_working(s_sisdb_worker *);


#endif  /* _SISDB_H */
