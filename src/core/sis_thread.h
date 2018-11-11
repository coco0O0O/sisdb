#ifndef _SIS_THREAD_H
#define _SIS_THREAD_H

#include <sis_core.h>
#include <sis_time.h>
#include <sis_list.h>

// ��������ģʽ ����������У�������ʱ�������
#define SIS_WORK_MODE_NONE     0
#define SIS_WORK_MODE_GAPS     1  // ����������У���Ҫ��Ͽ�ʼ�ͽ���ʱ��
#define SIS_WORK_MODE_PLANS    2  // ��ʱ���б����У�ʱ�侫ȷ������
#define SIS_WORK_MODE_ONCE     3  // ֻ����һ��

typedef struct s_sis_plan_task {
	int  		 		work_mode; 
	bool         		working;          // �˳�ʱ����Ϊfalse 
	s_sis_struct_list  *work_plans;       // plans-work ��ʱ���� uint16 ������
	s_sis_time_gap      work_gap; 		  // always-work ѭ�����е�����

	s_sis_mutex_t 		mutex;  // ��
	s_sis_thread_id_t   work_pid;

	s_sis_wait 			wait;   //   �߳��ڲ���ʱ����
	void(*call)(void *);        // ==NULL ���ͷŶ�Ӧ�ڴ�
} s_sis_plan_task;

s_sis_plan_task *sis_plan_task_create();

void sis_plan_task_destroy(s_sis_plan_task *task_);

bool sis_plan_task_start(s_sis_plan_task *task_,SIS_THREAD_START_ROUTINE func_, void* val_);

bool sis_plan_task_working(s_sis_plan_task *task_);
bool sis_plan_task_execute(s_sis_plan_task *task_);  // ���ʱ���Ƿ��ˣ�����ִ�оͷ�����


#endif //_SIS_TIME_H