/*
 * ========================================================================================
 *
 *   Filename   :  debug.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月11日17时47分41秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "debug.h"

static void print_stack_trace();

static elf_t kernel_elf;

void init_debug()
{
	/*
		从GRUB提供的信息中获取到内核符号表和代码地址信息 
	 */
	kernel_elf = elf_from_multiboot(glb_mboot_ptr);
}

void print_cur_status()
{
	static int round = 0;
	unsigned short reg1, reg2, reg3, reg4;

	asm volatile(
			"mov %%cs, %0;"
			"mov %%ds, %1;"
			"mov %%es, %2;"
			"mov %%ss, %3;"
			: "=m"(reg1), "=m"(reg2), "=m"(reg3), "=m"(reg4)
			);
	/* 打印当前的运行级别 */
	printk("%d: @ring %d\n", round, reg1 & 0x03);
	printk("%d:  cs = %x\n", round, reg1);
	printk("%d:  ds = %x\n", round, reg2);
	printk("%d:  es = %x\n", round, reg3);
	printk("%d:  ss = %x\n", round, reg4);
}

void panic(const char *msg)
{
	printk("*** System panic: %s\n", msg);
	printk_stack_trace();
	printk("***\n");

	/* 致命错误发生后打印栈信息后停止在这里 */
	while(1);
}

void print_stack_trace()
{
	unsigned int *ebp, *eip;

	asm volatile("mov %%ebp, %0" : "=r"(ebp));
	while(ebp)
	{
		eip = ebp + 1;
		printk(" [0x%x] %s\n", *eip, elf_lookup_symbol(*eip, &kernel_elf));
		ebp = (unsigned int *)*ebp;
	}
}







