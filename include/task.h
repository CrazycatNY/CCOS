/*
 * ========================================================================================
 *
 *   Filename   :  task.h
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年03月11日16时02分06秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __TASK_H
#define __TASK_H

#include "pmm.h"
#include "vmm.h"


/*
	进程状态描述
 */
typedef enum task_state{
	TASK_UNINIT = 0,
	TASK_SLEEPING = 1,
	TASK_RUNNABLE = 2,
	TASK_ZOMBIE = 3,
}task_state;

/*
	内核线程的上下文切换保存的信息
 */
struct context{
	unsigned int esp;
	unsigned int ebp;
	unsigned int ebx;
	unsigned int esi;
	unsigned int edi;
	unsigned int eflags;
};

/*
	进程内存地址结构
 */
struct mm_struct{
	pgd_t *pgd_dir;
};

/*
	进程控制块
 */
struct task_struct{
	volatile task_state state;
	int pid;
	void *stack;
	struct mm_struct *mm;
	struct context context;
	struct task_struct *next;
};

/*
	全局pid
 */
extern int now_pid;

/*
	内核线程创建
 */
int kernel_thread(int (*fn)(void *), void *arg);

/*
	线程退出函数
 */
void kthread_exit();

#endif

