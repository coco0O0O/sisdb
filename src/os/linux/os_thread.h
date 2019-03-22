#ifndef _OS_THREAD_H
#define _OS_THREAD_H

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include <os_time.h>
#include <stdio.h>
#include <errno.h>

// ����ʱ��ŷ��ظ�ֵ�����ǿ���˳������ظ�ֵ
#define SIS_ETIMEDOUT ETIMEDOUT
// �̳߳�������
#define SIS_THREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define SIS_THREAD_PROC void *

// �߳����Ͷ���
typedef void * (SIS_THREAD_START_ROUTINE)(void *); 
typedef pthread_mutex_t s_sis_mutex_t;
typedef pthread_cond_t s_sis_cond_t;
typedef pthread_t s_sis_thread_id_t;

typedef struct s_sis_thread {	
	int                timeout; 
	bool               working;  // 1 ������ 0 ����
	s_sis_thread_id_t  thread_id;  
} s_sis_thread;

// �̺߳�������
#ifdef __cplusplus
extern "C" {
#endif
bool sis_thread_create(SIS_THREAD_START_ROUTINE func_, void* val_, s_sis_thread *thread_);
// �ȴ��߳̽���
void sis_thread_finish(s_sis_thread *thread_);
void sis_thread_join(s_sis_thread *thread_); 
// ������linux���ͷ��߳���Դ
void sis_thread_clear(s_sis_thread *thread);
//��ȡ�߳�ID
s_sis_thread_id_t sis_thread_self(); 
// ɱ��
#ifdef __cplusplus
}
#endif
#define sis_thread_kill kill

// ����������
// windows֧�ֵ���
// PTHREAD_MUTEX_RECURSIVE_NP  ��Ƕ����
// linux֧�ֵ���
#ifndef  __APPLE__
#define  SIS_PTHREAD_MUTEX_NORMAL      PTHREAD_MUTEX_FAST_NP // ��ͨ��
#define  SIS_PTHREAD_MUTEX_RECURSIVE   PTHREAD_MUTEX_RECURSIVE_NP // Ƕ����
#define  SIS_PTHREAD_MUTEX_ERRORCHECK  PTHREAD_MUTEX_ERRORCHECK_NP // ������
#define  SIS_PTHREAD_MUTEX_TIMED_NP    PTHREAD_MUTEX_TIMED_NP
#define  SIS_PTHREAD_MUTEX_ADAPTIVE_NP PTHREAD_MUTEX_ADAPTIVE_NP  // ��Ӧ��
#else
#define  SIS_PTHREAD_MUTEX_NORMAL    PTHREAD_MUTEX_NORMAL // ��ͨ��
#define  SIS_PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE // Ƕ����
#define  SIS_PTHREAD_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK // ������
#endif



int  sis_mutex_create(s_sis_mutex_t *mutex_);
#define sis_mutex_destroy 	pthread_mutex_destroy
#define sis_mutex_lock    	pthread_mutex_lock
#define sis_mutex_unlock  	pthread_mutex_unlock
#define sis_mutex_init    	pthread_mutex_init
#define sis_mutex_trylock   pthread_mutex_trylock

#define sis_pthread_cond_init	pthread_cond_init
#define sis_pthread_mutex_init  pthread_mutex_init

typedef struct s_sis_wait {
	bool          used;  // �Ƿ�����ʹ��
	s_sis_cond_t  cond;  
	s_sis_mutex_t mutex;
} s_sis_wait;
// ��˼���ý� ���ܶ�̬����

#ifdef __cplusplus
extern "C" {
#endif
void sis_thread_wait_create(s_sis_wait *wait_);
void sis_thread_wait_destroy(s_sis_wait *wait_);
void sis_thread_wait_kill(s_sis_wait *wait_);
void sis_thread_wait_notice(s_sis_wait *wait_);

// ����������ʱ��ʽһ����ʱ����1�����ϣ�����û�б�Ҫ��ô���ӣ�����delay��λΪ��
int   sis_thread_wait_sleep(s_sis_wait *wait_, int delay_);
int   sis_thread_wait_sleep_msec(s_sis_wait *wait_, int msec_);
void  sis_thread_wait_start(s_sis_wait *wait_);
void  sis_thread_wait_stop(s_sis_wait *wait_);
#ifdef __cplusplus
}
#endif

#endif /* _SIS_THREAD_H */
