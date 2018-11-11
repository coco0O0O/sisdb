#ifndef _OS_THREAD_H
#define _OS_THREAD_H

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include <os_time.h>

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

// �̺߳�������

bool sis_thread_create(SIS_THREAD_START_ROUTINE func_, void* val_, s_sis_thread_id_t *thread_);
// �ȴ��߳̽���
void sis_thread_join(s_sis_thread_id_t thread); 
// ������linux���ͷ��߳���Դ
void sis_thread_clear(s_sis_thread_id_t thread);
//��ȡ�߳�ID
s_sis_thread_id_t sis_thread_self(); 
// ɱ��
#define sis_thread_kill kill

// ����������
// windows֧�ֵ���
// PTHREAD_MUTEX_RECURSIVE_NP  ��Ƕ����
// linux֧�ֵ���
#ifndef  __APPLE__
#define  SIS_PTHREAD_MUTEX_NORMAL    PTHREAD_MUTEX_FAST_NP // ��ͨ��
#define  SIS_PTHREAD_MUTEX_RECURSIVE PTHREAD_MUTEX_RECURSIVE_NP // Ƕ����
#define  SIS_PTHREAD_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK_NP // ������
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

// ���һд������
typedef struct s_sis_mutex_rw {
	s_sis_mutex_t mutex_s;
	volatile bool try_write_b;
	volatile int reads_i;
	volatile int writes_i;
} s_sis_mutex_rw;

int  sis_mutex_rw_create(s_sis_mutex_rw *mutex_);
void sis_mutex_rw_destroy(s_sis_mutex_rw *mutex_);
void sis_mutex_rw_lock_r(s_sis_mutex_rw *mutex_);
void sis_mutex_rw_unlock_r(s_sis_mutex_rw *mutex_);
void sis_mutex_rw_lock_w(s_sis_mutex_rw *mutex_);
void sis_mutex_rw_unlock_w(s_sis_mutex_rw *mutex_);

typedef struct s_sis_wait {
	s_sis_cond_t  cond;  
	s_sis_mutex_t mutex;
} s_sis_wait;

void sis_thread_wait_create(s_sis_wait *wait_);
void sis_thread_wait_destroy(s_sis_wait *wait_);
void sis_thread_wait_kill(s_sis_wait *wait_);

// ����������ʱ��ʽһ����ʱ����1�����ϣ�����û�б�Ҫ��ô���ӣ�����delay��λΪ��
int   sis_thread_wait_sleep(s_sis_wait *wait_, int delay_);
void  sis_thread_wait_start(s_sis_wait *wait_);
void  sis_thread_wait_stop(s_sis_wait *wait_);


#endif /* _SIS_THREAD_H */
