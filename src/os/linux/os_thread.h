#ifndef _OS_THREAD_H
#define _OS_THREAD_H

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include <os_time.h>

#include <errno.h>

#define STS_ETIMEDOUT ETIMEDOUT
// �̳߳�������
#define STS_THREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define STS_THREAD_PROC void *

// �߳����Ͷ���
typedef void * (STS_THREAD_START_ROUTINE)(void *); 
typedef pthread_mutex_t s_sts_mutex_t;
typedef pthread_cond_t s_sts_cond_t;
typedef pthread_t s_sts_thread_id_t;

// �̺߳�������

bool sts_thread_create(STS_THREAD_START_ROUTINE func_, void* val_, s_sts_thread_id_t *thread_);
// �ȴ��߳̽���
void sts_thread_join(s_sts_thread_id_t thread); 
// ������linux���ͷ��߳���Դ
void sts_thread_clear(s_sts_thread_id_t thread);
//��ȡ�߳�ID
s_sts_thread_id_t sts_thread_self(); 
// ɱ��
#define sts_thread_kill kill

// ����������
int  sts_mutex_create(s_sts_mutex_t *mutex_);
#define sts_mutex_destroy pthread_mutex_destroy
#define sts_mutex_lock    pthread_mutex_lock
#define sts_mutex_unlock  pthread_mutex_unlock
#define sts_mutex_init    pthread_mutex_init

// ���һд������
typedef struct s_sts_mutex_rw {
	s_sts_mutex_t mutex_s;
	volatile bool try_write_b;
	volatile int reads_i;
	volatile int writes_i;
} s_sts_mutex_rw;

int  sts_mutex_rw_create(s_sts_mutex_rw *mutex_);
void sts_mutex_rw_destroy(s_sts_mutex_rw *mutex_);
void sts_mutex_rw_lock_r(s_sts_mutex_rw *mutex_);
void sts_mutex_rw_unlock_r(s_sts_mutex_rw *mutex_);
void sts_mutex_rw_lock_w(s_sts_mutex_rw *mutex_);
void sts_mutex_rw_unlock_w(s_sts_mutex_rw *mutex_);

typedef struct s_sts_wait {
	s_sts_cond_t cond;  
	s_sts_mutex_t mutex;
} s_sts_wait;

void sts_thread_wait_create(s_sts_wait *wait_);
void sts_thread_wait_destroy(s_sts_wait *wait_);
void sts_thread_wait_kill(s_sts_wait *wait_);

// ����������ʱ��ʽһ����ʱ����1�����ϣ�����û�б�Ҫ��ô���ӣ�����delay��λΪ��
int   sts_thread_wait_sleep(s_sts_wait *wait_, int delay_);
void  sts_thread_wait_start(s_sts_wait *wait_);
void  sts_thread_wait_stop(s_sts_wait *wait_);


#endif /* _STS_THREAD_H */
