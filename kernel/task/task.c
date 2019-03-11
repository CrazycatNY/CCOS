/*
 * ========================================================================================
 *
 *   Filename   :  task.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年03月11日16时46分48秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */

#include "gdt.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"
#include "sched.h"
#include "string.h"
#include "debug.h"
#include "task.h"

/*
	全局pid值
 */
int now_pid = 0;

/*
	内核线程创建
 */
int kernel_thread(int (*fn)(void *), void *arg)
{
	struct task_struct *new_task = (struct task_struct *)kmalloc(STACK_SIZE);
	assert(new_task != NULL, "kern_thread: kmalloc error");

	bzero(new_task, sizeof(struct task_struct));

	new_task->state = TASK_RUNNABLE;
	new_task->stack = current;
	new_task->pid = now_pid++;
	new_task->mm = NULL;

	unsigned int *stack_top = (unsigned int *)((unsigned int )new_task + STACK_SIZE);
	*(--stack_top) = (unsigned int)arg;
	*(--stack_top) = (unsigned int)kthread_exit;
	*(--stack_top) = (unsigned int)fn;

	new_task->context.esp = (unsigned int)new_task + STACK_SIZE - sizeof(unsigned int) * 3;
	/*
		设置新任务的标志寄存器未屏蔽中断，很重要
	 */
	new_task->context.eflags = 0x200;
	new_task->next = running_proc_head;

	/*
		找到当前任务队列，插入到末尾
	 */

	struct task_struct *tail = running_proc_head;
	assert(tail != NULL, "Must init sched");

	while(tail->next != running_proc_head)
	{
		tail = tail->next;
	}
	tail->next = new_task;

	return new_task->pid;
}

void kthread_exit()
{
	register unsigned int val asm("eax");

	printk("Thread exited with value %d\n", val);

	while(1);
}





