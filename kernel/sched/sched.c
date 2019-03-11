/*
 * ========================================================================================
 *
 *   Filename   :  sched.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年03月11日16时27分52秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "sched.h"

#include "pmm.h"
#include "heap.h"
#include "debug.h"

/*
	创建三个进程链表
 */
struct task_struct *running_proc_head = NULL;

struct task_struct *wait_proc_head = NULL;

struct task_struct *current = NULL;

void init_sched()
{
	/*
		当前运行的进程信息结构体放在栈底
	 */
	current = (struct task_struct *)(kern_stack_top - STACK_SIZE);

	current->state = TASK_RUNNABLE;
	current->pid = now_pid++;
	current->stack = current;
	current->mm = NULL;

	current->next = current;

	running_proc_head = current;
}

void schedule()
{
	if(current){
		change_task_to(current->next);
	}
}

void change_task_to(struct task_struct *next)
{
	if(current != next)
	{
		struct task_struct *prev = current;
		current = next;
		switch_to(&(prev->context), &(current->context));
	}
}








