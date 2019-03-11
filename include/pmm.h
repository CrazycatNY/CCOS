/*
 * ========================================================================================
 *
 *   Filename   :  pmm.h
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月29日16时21分27秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __PMM_H
#define __PMM_H

#include "multiboot.h"

/*
	线程栈的大小
 */
#define STACK_SIZE	8192

/*
	支持的最大物理内存(512M)
 */
#define PMM_MAX_SIZE	0x20000000

/*
	物理内存页大小(4KB)
 */
#define PMM_PAGE_SIZE	0x1000

/*
	最大支持的物理页个数
 */
#define PAGE_MAX_SIZE	(PMM_MAX_SIZE/PMM_PAGE_SIZE)


/*
	页掩码， 按4096对齐地址
 */
#define PHY_PAGE_MASK 0xFFFFF000

/*
	内核文件在内存中的起始和结束位置在链接器脚本中，要求链接器定义
 */
extern unsigned char kern_start[];
extern unsigned char kern_end[];

/* 
	动态分配物理内存页的计数
 */
extern unsigned int phy_page_count;

extern unsigned int kern_stack_top;

/* 输出bios提供的物理内存布局 */
void show_memory_map();

/*
	初始化物理内存管理，内核代码以后的内存，按4K每页，将页首地址存放在栈中
 */
void init_pmm();

/*
	返回一个内存页的物理地址
 */
unsigned int pmm_alloc_page();

/*
	释放申请的内存， 首地址存入栈
 */
void pmm_free_page(unsigned int p);

#endif

