
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _STS_DB_H
#define _STS_DB_H

#include "sts_map.h"
#include "sts_table.h"

//#define STS_DB_DATETYPE_STR "json,bin,zip,array" // 
#define STS_DB_DATETYPE_STR "json,struct" // 

#pragma pack(push,1)
typedef struct s_sts_map_define{
	const char *key;
	uint16_t	uid;
	uint32_t    size;
}s_sts_map_define;
#pragma pack(pop)

#define s_sts_db s_sts_map_pointer 

#define STATIC_STS_DB

#ifdef STATIC_STS_DB
// extern s_sts_db *__sys_sts_db;
void sts_db_create();  
void sts_db_destroy();  
//ȡ���ݺ�д����
s_sts_table *sts_db_get_table(const char *name_); // �ɴ˺����ж����ݿ��Ƿ���ָ����
void sts_db_install_table(s_sts_table *);

s_sts_map_define *sts_db_find_map_define(const char *name_);

#else
s_sts_db *sts_db_create();  //���ݿ�����ƣ�Ϊ�ս���һ��sys�����ݿ���
void sts_db_destroy(s_sts_db *);  //�ر�һ�����ݿ�
//ȡ���ݺ�д����
s_sts_table *sts_db_get_table(s_sts_db *, const char *name_); //name -- table name
void sts_db_install_table(s_sts_db *, s_sts_table *); //װ���
#endif



#endif  /* _STS_DB_H */
