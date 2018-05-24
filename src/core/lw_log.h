#ifndef _LW_LOG_H
#define _LW_LOG_H

#include <stdio.h>
#include "lw_thread.h"
#include "lw_public.h"

#define LOG_DEBUG 1001

///////////////////////
//LOG_DEBUG�����³������ʹ��,Ĭ�ϴ���1000,ͬһ����ʹ��һ����ţ���ʽ�������ٶ��壬�Ϳ��Բ���ʾ��LOG
//1���ٵĿ��������������˳�������������ʾ��Ϣ��***������ѭ�������***
//2����������Ҫ�̵߳Ŀ��ܳ�������е���ʾ
//3��һ��������Ϣ
//4�ٴ�һ��������Ϣ
//5���������̺�������Ϣ
//6-9������Ҫ�Դ�ӡ��������Ϣ
///////////////////////

typedef struct s_log {
	int logLevel;
	int threadid;
	char funcname[255];

	pthread_mutex_t muLog;
	FILE *logFile;
	char filename[255];
	char buffer[2048];
	int  maxsize;
} s_log;

extern struct s_log __log;

void log_open(const char *file, int level, int ms);
void log_close();
void log_print(const char *fmt, ...);

int writefile(char *name, void *value, size_t len);

#ifdef LOG_DEBUG
#define LOG(level) 	snprintf(__log.funcname,sizeof(__log.funcname), "%s",__FUNCTION__); if ((level == LOG_DEBUG)||( level <= __log.logLevel && level < 10 )) log_print
#else
#define LOG(level) 	snprintf(__log.funcname,sizeof(__log.funcname),"%s",__FUNCTION__); if ( level <= __log.logLevel && level < 10 ) log_print
#endif

#endif //_LW_LOG_H


