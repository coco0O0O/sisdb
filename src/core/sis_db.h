﻿#ifndef _SIS_DB_H
#define _SIS_DB_H

#include <sis_core.h>
#include <sis_map.h>
#include <sis_dynamic.h>
#include <sis_conf.h>
#include <sis_csv.h>
#include <sis_method.h>

/////////////////////////////////////////////////////////
//  数据格是定义 
/////////////////////////////////////////////////////////
// 下面以字节流方式传递
// #define SISDB_FORMAT_SDB     0x0001   // 后面开始为数据 keylen+key + sdblen+sdb + ssize + 二进制数据
// #define SISDB_FORMAT_SDBS    0x0002   // 后面开始为数据 count + keylen+key + sdblen+sdb + ssize + 二进制数据
// #define SISDB_FORMAT_KEY     0x0004   // 后面开始为数据 keylen+key + ssize + 二进制数据
// #define SISDB_FORMAT_KEYS    0x0008   // 后面开始为数据 count + keylen+key + ssize + 二进制数据
#define SISDB_FORMAT_STRUCT  0x0001   // 结构化原始数据
#define SISDB_FORMAT_BITZIP  0x0002   // 相关压缩数据  
#define SISDB_FORMAT_BUFFER  0x0004   // 二进制数据流  

#define SISDB_FORMAT_BYTES   0x00FF   // 二进制数据流  
// 下面必须都是可见字符
#define SISDB_FORMAT_STRING  0x0100   // 直接传数据 json文档 {k1:{},k2:{}}
#define SISDB_FORMAT_JSON    0x0200   // 直接传数据 json文档 {k1:{},k2:{}}
#define SISDB_FORMAT_ARRAY   0x0400   // 数组格式 {k1:[]}
#define SISDB_FORMAT_CSV     0x0800   // csv格式，需要处理字符串的信息
#define SISDB_FORMAT_CHARS   (SISDB_FORMAT_STRING|SISDB_FORMAT_JSON|SISDB_FORMAT_ARRAY|SISDB_FORMAT_CSV) 

typedef struct s_sis_db_chars
{
	const char   *kname;
	const char   *sname;
	uint32        size;
	void         *data;     
} s_sis_db_chars;

typedef struct s_sis_db_bytes
{
	int        kidx;
	int        sidx;
	uint32     size;
	void      *data;     
} s_sis_db_bytes;

typedef struct s_sis_db_incrzip
{
	uint8      init;
	uint32     size;
	void      *data;     
} s_sis_db_incrzip;

#define SIS_DB_STREAM_CHARS    1
#define SIS_DB_STREAM_BYTES    2
#define SIS_DB_STREAM_INCRZIP  3

// 得到参数中是字符串还是字节流
int sis_db_get_format(s_sis_sds argv_);
// 得到格式
int sis_db_get_format_from_node(s_sis_json_node *node_, int default_);

// 从二进制转字符串
s_sis_sds sis_db_format_sds(s_sis_dynamic_db *db_, const char *key_, int iformat_, 
	const char *in_, size_t ilen_, int isfields_);


/////////////////////////////////////////////////////////
// 多数据结构 统一排序输出的定义 
/////////////////////////////////////////////////////////
// 先根据需要处理的结构体获得最小时间尺度
// (默认传入的数据都是顺序的) 
// 对每一条传入数据转换其时间格式为最小尺度时间 vsort 以此为排序键
// 对所有的键数据进行最小时间排序 当第一个key数据返回完毕时
// 回调告诉上层key需要新的数据 新数据补充后继续输出数据 
// 上层没有新数据 删除该key 用另一个最小的时间为判断继续
/////////////////////////////////////////////////////////

#define SIS_SUBDB_NONE     0
#define SIS_SUBDB_INSERT   1
#define SIS_SUBDB_TAIL     2

typedef struct s_sis_subdb_unit
{
	s_sis_sds          kname;    
	s_sis_dynamic_db  *db;     // 指针
	s_sis_struct_list *vlist;  // 数据区
	s_sis_struct_list *vmsec;  // 转化后的序列号 每条记录一个数
	struct s_sis_subdb_unit *next;  // 下一个 unit 按第一个vmsec确定位置
	struct s_sis_subdb_unit *prev;  // 下一个 unit 按第一个vmsec确定位置
} s_sis_subdb_unit;

#define SIS_SUBDB_NONE     0
#define SIS_SUBDB_START    1
#define SIS_SUBDB_STOP     2
#define SIS_SUBDB_STOPED   3
// 默认传入的数据都是顺序的 
typedef struct s_sis_subdb_cxt
{
	char                 cur_scale;  // == 0 init 后更新
	s_sis_map_list      *work_sdbs;  // s_sis_dynamic_db 数据表格式定义

	s_sis_subdb_unit    *head;
	s_sis_subdb_unit    *tail;
	s_sis_map_pointer   *work_units; // s_sis_subdb_unit 来源数据定义 key.sdb 为键值

	int                  work_status;   // 0 初始化 1 start 2 wait stop 3 stoped
	s_sis_thread         work_thread;  // 工作线程
	
	void                *cb_source;
	sis_method_define   *cb_sub_start;     // 全部数据开始
	sis_method_define   *cb_sub_stop;      // 全部数据结束
	// sis_method_define   *cb_sub_begin;     // 某天数据开始
	// sis_method_define   *cb_sub_end;       // 某天数据结束
	sis_method_define   *cb_key_stop;      // 某个key数据消耗完毕
	sis_method_define   *cb_key_bytes;     // 某个key数据 s_sis_db_chars

} s_sis_subdb_cxt;

s_sis_subdb_cxt *sis_subdb_cxt_create();
void sis_subdb_cxt_destroy(s_sis_subdb_cxt *);

void sis_subdb_cxt_clear(s_sis_subdb_cxt *);

// void sis_subdb_cxt_set_keys(s_sis_subdb_cxt *, s_sis_sds );
void sis_subdb_cxt_init_sdbs(s_sis_subdb_cxt *, const char *,size_t);

void sis_subdb_cxt_add_data(s_sis_subdb_cxt *, const char *key_, void *in_, size_t ilen_);
// 开始订阅
void sis_subdb_cxt_sub_start(s_sis_subdb_cxt *);
// 结束或中断订阅
void sis_subdb_cxt_sub_stop(s_sis_subdb_cxt *);


#endif //_SIS_DB_H

