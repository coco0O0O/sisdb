
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

#define SIS_MAXLEN_TABLE 32
#define SIS_MAXLEN_CODE  9

// �Դ���Ϊ����������ÿ���������������Ϣ�������
// �۸�С���� Ĭ��Ϊ 2  ͳһ��info����decimal��ȡ
// �۸�λ   Ĭ��Ϊ 1  ���رң���Ҫ���� ǧ�����
//			 ͳһ��info����coinunit��ȡ
// �ɽ�����λ Ĭ��Ϊ 100 = 1��  ͳһ��info����volunit��ȡ
//  		ָ����λ����Ϊ 10000 = ����
// ��λ   Ĭ��Ϊ 100 = ��Ԫ ͳһ��info����volunit��ȡ
//  		ָ����λ����Ϊ 10000 = ��Ԫ
// ����ʱ�� �Ӷ�Ӧ�г���trade-time�ֶλ�ȡ
// ����ʱ�� �Ӷ�Ӧ�г���work-time�ֶλ�ȡ

#define SIS_TABLE_EXCH  "exch"   // Ĭ�ϵ��г����ݱ�����
#define SIS_TABLE_EXCH  "info"   // Ĭ�ϵĹ�Ʊ���ݱ�����

// ����������Ϊ��Ҫ����������ھͱ���

#pragma pack(push,1)

// ÿ����Ʊ���������Ľṹ����Ϣ��
typedef struct s_sisdb_sys_exch {
	s_sis_time_pair     work_time;
	s_sis_struct_list  *trade_time; // [[930,1130],[1300,1500]] ����ʱ�䣬����������
}s_sisdb_sys_exch;

// ÿ����Ʊ���������Ľṹ����Ϣ��
typedef struct s_sisdb_sys_info {
	uint8   decimal;
	uint32  coinunit;
	uint32  volunit;	
	// s_sisdb_sys_exch *exch;  // �Ӵ˻�ȡworktime��tradetime;
}s_sisdb_sys_info;
// ������Ʊʱ���û�иù�Ʊ���룬����Ҫ�Զ���exch��info�����ɶ�Ӧ����Ϣ��ֱ�����ٴ�ˢ�£�
// �г����Ĭ��ȡ����ǰ��λ��

// sisdb����keyֱ�Ӷ�λ������ keyΪ SH.600600.DAY �ķ�ʽ���Ա�ﵽ��ߵ�Ч�ʣ�
typedef struct s_sisdb_stock {
	s_sis_db            *father;    // ���ڵ� ��ͨ�������ȡ table �ṹ��Ϣ
	s_sisdb_sys_exch    *exch; 
	s_sisdb_sys_info     info;      // ����Я������Ϣ,ÿ������һ�ݶ���������
	s_sis_collect_unit  *collect;   // ��������s_sis_table
	// s_sis_table collect->table;  // �����ݶ�Ӧ��tableָ������һ����Ϊ���ټ���������tableΪ��ʱ����dbs׷��һ������
}s_sisdb_stock;

typedef struct s_sis_db {
	s_sis_sds name;            // ���ݿ����� sisdb

	s_sis_sds conf;            // ��ǰ���ݱ��������Ϣ ����ʱ��

	s_sis_map_pointer  *dbs;     // ���ݱ���ֵ�� s_sis_table
	s_sis_map_pointer  *map;     // �ؼ��ֲ�ѯ��

//  ����Ĵ����߳�Ӧ�ý���һ���ṹ�������������������鷳
//  �ȿ�ʼ����worktimeʱһ������ʱ�����߳�
    int    save_mode;   // ���̷�ʽ�����ʱ��棬����ָ��ʱ���
    s_sis_struct_list  *save_plans;   // plans-work ��ʱ���� uint16 ������
    s_sis_time_gap      save_always;  // always-work ѭ�����е�����

	s_sis_thread_id_t save_pid;
	s_sis_mutex_t save_mutex;  // saveʱ������

	int save_format;   // �����ļ��ķ�ʽ

	s_sis_wait thread_wait; //�߳��ڲ���ʱ����

}s_sis_db;
#pragma pack(pop)


s_sis_db *sisdb_create(char *);  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sisdb_destroy(s_sis_db *);  //�ر�һ�����ݿ�


#endif  /* _SISDB_H */
