#ifndef _OS_THREAD_H
#define _OS_THREAD_H

// ���¼����ǲ���cpu�� ʵ�ʲ��Է���ָ��cpu����Ч�ʵ�
// #ifndef _GNU_SOURCE
// #define _GNU_SOURCE
// #endif
// #include <sched.h>
// #include <sys/sysinfo.h>

#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include <os_time.h>
#include <stdio.h>
#include <errno.h>

#include <os_fork.h>

// ����ʱ��ŷ��ظ�ֵ�����ǿ���˳������ظ�ֵ
#define SIS_ETIMEDOUT ETIMEDOUT
// �̳߳�������
#define SIS_THREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define SIS_THREAD_PROC void *

// �߳����Ͷ���
typedef void * (cb_thread_working)(void *); 
typedef pthread_mutex_t s_sis_mutex_t;
typedef pthread_rwlock_t s_sis_rwlock_t;

typedef pthread_cond_t s_sis_cond_t;
typedef pthread_t s_sis_thread_id_t;

typedef struct s_sis_thread {	
	int                timeout; 
	bool               working;  // 1 ������ 0 ����
	s_sis_thread_id_t  thread_id;  
	cb_thread_working *worker;
	void 			  *argv;
} s_sis_thread;

#ifdef __cplusplus
extern "C" {
#endif
// �̺߳�������
bool sis_thread_create(cb_thread_working func_, void* val_, s_sis_thread *thread_);
// �ȴ��߳̽���
void sis_thread_finish(s_sis_thread *thread_);
void sis_thread_join(s_sis_thread *thread_); 
// ������linux���ͷ��߳���Դ
void sis_thread_clear(s_sis_thread *thread);
//��ȡ�߳�ID
s_sis_thread_id_t sis_thread_self(); 
// ɱ��
#define sis_thread_kill kill
#ifdef __cplusplus
}
#endif

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

typedef struct s_sis_wait {
	unsigned char status; // 0 ���� 1 �˳�
	bool          used;   // �Ƿ�����ʹ��
	s_sis_cond_t  cond;  
	s_sis_mutex_t mutex;
} s_sis_wait;
// ��˼���ý� ���ܶ�̬����

#ifdef __cplusplus
extern "C" {
#endif

int  sis_mutex_create(s_sis_mutex_t *mutex_);
#define sis_mutex_destroy 	pthread_mutex_destroy
#define sis_mutex_lock    	pthread_mutex_lock
#define sis_mutex_unlock  	pthread_mutex_unlock
#define sis_mutex_init    	pthread_mutex_init
// ���� 0 ��ʾ���ɹ�
#define sis_mutex_trylock   pthread_mutex_trylock

int  sis_rwlock_init(s_sis_rwlock_t *rwlock_);
#define sis_rwlock_destroy 	  pthread_rwlock_destroy
#define sis_rwlock_lock_r     pthread_rwlock_rdlock
#define sis_rwlock_lock_w     pthread_rwlock_wrlock
#define sis_rwlock_unlock  	  pthread_rwlock_unlock
// ���� 0 ��ʾ���ɹ�
#define sis_rwlock_trylock_r   pthread_rwlock_tryrdlock
#define sis_rwlock_trylock_w   pthread_rwlock_trywrlock

int sis_mutex_wait_lock_r(s_sis_rwlock_t *rwlock_, int msec_);
int sis_mutex_wait_lock_w(s_sis_rwlock_t *rwlock_, int msec_);

void sis_thread_wait_create(s_sis_wait *wait_);
void sis_thread_wait_destroy(s_sis_wait *wait_);
void sis_thread_wait_init(s_sis_wait *wait_);
void sis_thread_wait_kill(s_sis_wait *wait_);
void sis_thread_wait_notice(s_sis_wait *wait_);

// ����������ʱ��ʽһ����ʱ����1�����ϣ�����û�б�Ҫ��ô���ӣ�����delay��λΪ��
int   sis_thread_wait_sleep(s_sis_wait *wait_, int delay_);
int   sis_thread_wait_sleep_msec(s_sis_wait *wait_, int msec_);
void  sis_thread_wait_start(s_sis_wait *wait_);
void  sis_thread_wait_stop(s_sis_wait *wait_);

//////////////////////////////////////
//  unlock
//////////////////////////////////////
// **** Ƶ������������������ ��ʱ����3�� ���Ӹ���Ч�������� ***** // 

// type������1,2,4��8�ֽڳ��ȵ�int���ͣ�����
// int8_t / uint8_t
// int16_t / uint16_t
// int32_t / uint32_t
// int64_t / uint64_t

// ���ظ���ǰ��ֵ����fetch�ټ��㣩
// type __sync_fetch_and_add (type *ptr, type value, ...)
// type __sync_fetch_and_sub (type *ptr, type value, ...)
// type __sync_fetch_and_or (type *ptr, type value, ...)
// type __sync_fetch_and_and (type *ptr, type value, ...)
// type __sync_fetch_and_xor (type *ptr, type value, ...)
// type __sync_fetch_and_nand (type *ptr, type value, ...)
// ���ظ��º��ֵ���ȼ�����fetch��
// type __sync_add_and_fetch (type *ptr, type value, ...)
// type __sync_sub_and_fetch (type *ptr, type value, ...)
// type __sync_or_and_fetch (type *ptr, type value, ...)
// type __sync_and_and_fetch (type *ptr, type value, ...)
// type __sync_xor_and_fetch (type *ptr, type value, ...)
// type __sync_nand_and_fetch (type *ptr, type value, ...)
// ����Ŀ���չ����(...)����ָ����Щ������Ҫmemory barrier,��ΪĿǰgccʵ�ֵ���full barrier��
// ������linux kernel �е�mb(),��ʾ�������֮ǰ�������ڴ�������ᱻ�������������֮��,���Կ����Ե����������


// ���������ṩԭ�ӵıȽϺͽ��������*ptr == oldval,�ͽ�newvalд��*ptr,
// ��һ����������Ȳ�д�������·���true.
// �ڶ��������ڷ��ز���֮ǰ��ֵ��
// bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)
// type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)

// ��*ptr��Ϊvalue������*ptr����֮ǰ��ֵ��
// type __sync_lock_test_and_set (type *ptr, type value, ...)
// ��*ptr��0
// void __sync_lock_release (type *ptr, ...)
#ifdef __cplusplus
}
#endif

typedef int s_sis_unlock_mutex;



#ifdef __cplusplus
extern "C" {
#endif

#define BCAS __sync_bool_compare_and_swap
#define VCAS __sync_val_compare_and_swap
#define ADDF __sync_add_and_fetch
#define SUBF __sync_sub_and_fetch
#define ANDF __sync_and_and_fetch

// static inline void sis_unlock_mutex_init(s_sis_unlock_mutex *um_, int init_)
// {
//     ANDF(&(*um_), 0);
// }
// static inline void sis_unlock_mutex_lock(s_sis_unlock_mutex *um_)
// {
//     while(!BCAS(&(*um_), 0, 1))
//     {
//         sis_sleep(1);
//     }
// }

// static inline void sis_unlock_mutex_unlock(s_sis_unlock_mutex *um_)
// {
//     while(!BCAS(&(*um_), 1, 0))
//     {
//         sis_sleep(1);
//     }
// }
// // 0 ��ס
// static inline int sis_unlock_mutex_try_lock(s_sis_unlock_mutex *um_)
// {
//     if (BCAS(&(*um_), 0, 1))
//     {
//         return 0;
//     }
//     return 1;
// }
// static inline int sis_unlock_mutex_try_unlock(s_sis_unlock_mutex *um_)
// {
//     if (BCAS(&(*um_), 1, 0))
//     {
//         return 0;
//     }
//     return 1;
// }
#ifdef __cplusplus
}
#endif

#endif /* _SIS_THREAD_H */
