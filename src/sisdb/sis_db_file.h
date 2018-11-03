
//******************************************************
// Copyright (C) 2018, Martin <seerlinecoin@gmail.com>
//*******************************************************

#ifndef _SIS_DB_FILE_H
#define _SIS_DB_FILE_H

#include "sis_core.h"
#include "sis_conf.h"
#include "sis_str.h"

#include "os_file.h"

#include "sis_db.h"
#include "sis_table.h"
#include "sis_collect.h"
#include "sis_db_io.h"
#include "sis_memory.h"

#define SIS_DB_FILE_VERSION   1    // 数据结构版本

#define SIS_DB_FILE_CONF "%s/%s.json"    // 数据结构定义
#define SIS_DB_FILE_MAIN "%s/%s/%s.sdb"   // 主数据库
#define SIS_DB_FILE_ZERO "%s/%s/%s.sdb.0" // 不完备的数据块
#define SIS_DB_FILE_AOF  "%s/%s.aof"     // 顺序写入

// #define SIS_DB_FILE_USE_CATCH     // 是否启用ZERO

#pragma pack(push,1)
typedef struct s_sis_sdb_head{
	uint32	size;    // 数据大小
	char    code[SIS_CODE_MAXLEN];
	uint8	format;  // 数据格式 json array struct zip 
                // 还有一个数据结构是 refer 存储 catch 和 zip 三条记录
}s_sis_sdb_head;

typedef struct s_sis_aof_head{
	uint32	size;    // 数据大小
	char    code[SIS_CODE_MAXLEN];
	uint8	format;  // 数据格式 json array struct zip 
	                 // 还有一个数据结构是 refer 存储 catch 和 zip 三条记录
	char    table[SIS_TABLE_MAXLEN];
}s_sis_aof_head;

#pragma pack(pop)

//------v1.0采用全部存盘的策略 ---------//
// 到时间保存
bool sis_db_file_save(const char *dbpath_, s_sis_db *db_);

bool sis_db_file_saveto(const char *dbpath_, s_sis_db *db_, int format_, const char *tb_);

bool sis_db_file_save_aof(const char *dbpath_, s_sis_db *db_, 
            int format_, const char *tb_, const char *key_, 
            const char *val_, size_t len_);
bool sis_db_file_save_conf(const char *dbpath_, s_sis_db *db_);
// 开机时加载数据
bool sis_db_file_load(const char *dbpath_, s_sis_db *db_);

// 检查本地配置和数据库存盘的结构是否一致，如果不一致返回错误
// bool sis_db_file_check(const char *dbpath_, s_sis_db *db_);

// bool sisdb_file_save_json(const char *service_, const char *db_);
// bool sisdb_file_load_json(const char *service_, const char *db_);

#endif  /* _SIS_DB_FILE_H */
