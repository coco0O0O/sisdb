
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

#define SIS_MAXLEN_CODE  9
#define SIS_MAXLEN_TABLE 32
#define SIS_MAXLEN_KEY   (SIS_MAXLEN_CODE + SIS_MAXLEN_TABLE)

#define SIS_MARKET_STATUS_NOINIT    0
#define SIS_MARKET_STATUS_INITED    1  // ��������״̬
#define SIS_MARKET_STATUS_INITING   2  // ��ʼ��ʼ��
#define SIS_MARKET_STATUS_CLOSE     3  // ������,0&3 ��״̬�¿��Գ�ʼ��

// �Դ���Ϊ����������ÿ���������������Ϣ�������
// �۸�С���� Ĭ��Ϊ 2  ͳһ��info����dot��ȡ
// �۸�λ   Ĭ��Ϊ 1  ���رң���Ҫ���� ǧ�����
//			 ͳһ��info����coinunit��ȡ
// �ɽ�����λ Ĭ��Ϊ 100 = 1��  ͳһ��info����volunit��ȡ
//  		ָ����λ����Ϊ 10000 = ����
// ��λ   Ĭ��Ϊ 100 = ��Ԫ ͳһ��info����volunit��ȡ
//  		ָ����λ����Ϊ 10000 = ��Ԫ
// ����ʱ�� �Ӷ�Ӧ�г���trade-time�ֶλ�ȡ
// ����ʱ�� �Ӷ�Ӧ�г���work-time�ֶλ�ȡ

#define SIS_DEFAULT_EXCH  "00"   // Ĭ�ϵ��г���Ŵ洢ȱʡ����

#define SIS_TABLE_EXCH  "exch"   // Ĭ�ϵ��г����ݱ�����
#define SIS_TABLE_INFO  "info"   // Ĭ�ϵĹ�Ʊ���ݱ�����

// ����������Ϊ��Ҫ����������ھͱ���
#define SIS_TRADETIME_MAX_NUM  4   // Ĭ�ϵĹ�Ʊ���ݱ�����

#pragma pack(push,1)

typedef struct s_sisdb_cfg_info {
	uint8   dot;       // ����price������
	uint32  prc_unit;  // ����price������
	uint32  vol_unit;  // ����volume������	
}s_sisdb_cfg_info;

typedef struct s_sisdb_cfg_exch {
	char    market[3]; 
	uint8  	status;      // ״̬
	time_t  init_time;    // �����յ�now��ʱ��
	uint32  init_date;    // �����յ�now��ʱ��
	s_sis_time_pair  work_time; // ��ʼ��ʱ�� ��λ���� 900 
	uint8   		 trade_slot; // ����ʵ��ʱ���
	s_sis_time_pair  trade_time[SIS_TRADETIME_MAX_NUM]; // [[930,1130],[1300,1500]] ����ʱ�䣬����������
}s_sisdb_cfg_exch;

// ������Ʊʱ���û�иù�Ʊ���룬����Ҫ�Զ���exch��info�����ɶ�Ӧ����Ϣ��ֱ�����ٴ�ˢ�£�
// �г����Ĭ��ȡ����ǰ��λ��
// ------------------------------------------------------ //
// ����Ϊ�˱����������ݹ��࣬���������б�������info���⣬�´�����info�����ǲ�ͬ�ģ����򷵻�ָ��
// ------------------------------------------------------ //

typedef struct s_sis_db {
	s_sis_sds name;            // ���ݿ����� sisdb

	s_sis_sds conf;            // ��ǰ���ݱ��������Ϣ ����ʱ��

	s_sis_map_pointer  *dbs;          // ���ݱ���ֵ�� s_sisdb_table ����Ϊ���ݱ���

	s_sis_map_pointer  *cfg_exchs;    // �г�����Ϣ һ���г�һ����¼ Ĭ�ϼ�¼Ϊ ��00��  s_sisdb_cfg_exch
	s_sis_struct_list  *cfg_infos;    // ��Ʊ����Ϣ ʵ�ʵ�info���� ��һ��ΪĬ�����ã���ͬ������ s_sisdb_cfg_info

	s_sis_map_pointer  *collects;     // ���ݼ��ϵ��ֵ�� s_sisdb_collect ����ʵ�ʴ�����ݣ�����Ϊ��Ʊ����x���ݱ���

	s_sis_map_pointer  *map;          // �ؼ��ֲ�ѯ��
	
	bool   loading;  // ���ݼ�����

	int 				save_format;   // �����ļ��ķ�ʽ
	s_sis_plan_task    *save_task;

	s_sis_plan_task    *init_task;

}s_sis_db;

#pragma pack(pop)


s_sis_db *sisdb_create(char *);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sisdb_destroy(s_sis_db *);  //�ر�һ�����ݿ�

s_sisdb_cfg_exch *sisdb_config_create_exch(s_sis_db *db_, const char *code_);
s_sisdb_cfg_info *sisdb_config_create_info(s_sis_db *db_, const char *code_);

void sisdb_config_check(s_sis_db *db_, const char *key_, void *src_);

uint16 sisdb_ttime_to_trade_index_(uint64 ttime_, s_sis_struct_list *tradetime_);
uint64 sisdb_trade_index_to_ttime_(int date_, int idx_, s_sis_struct_list *tradetime_);

uint16 sisdb_ttime_to_trade_index(uint64 ttime_, s_sisdb_cfg_exch *cfg_);
uint64 sisdb_trade_index_to_ttime(int date_, int idx_, s_sisdb_cfg_exch *cfg_);

int sis_from_node_get_format(s_sis_db *db_, s_sis_json_node *node_);


#endif  /* _SISDB_H */
