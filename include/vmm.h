/*
 * ========================================================================================
 *
 *   Filename   :  vmm.h
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年02月20日17时33分32秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __VMM_H
#define __VMM_H

#include "idt.h"

/* 内核地址偏移 */
#define PAGE_OFFSET		0xC0000000

/*
	P	位0是存在(present)标志，用于指明表项对地址转换是否有效

	P = 1 表示有效；
	P = 0 表示无效

	在页转换过程中，如果说涉及的页目录或页表的表项无效，则会导致一个异常
	如果P = 0，那么除表示表项无效外，其余位可供程序自由使用
	例如，操作系统可以使用这些位来保存已存储在磁盘上的页面的序号
 */
#define PAGE_PRESENT	0x01

/*
	R/W	位1是读写标志位。
	1 表示页面可以被读写或执行
	0 表示页面只读或可执行
	当处理器运行在超级用户特权级0，1或2时，则RW位不起作用
	页目录项中的R/W位对其所映射的所有页面起作用
 */
#define PAGE_WRITE		0x2

/*
	U/S	位2 是用户/超级用户（User/Supervisor）标志
	1，表示运行在任何特权级别上的程序都可以访问该页面
	0，表示页面只能被运行在超级用户特权级（1，2,0）上的程序访问
	页目录项中的U/S位对其所映射的所有页面起作用
 */
#define PAGE_USER		0x04

/*
	虚拟页大小
 */
#define PAGE_SIZE		4096

/*
	页掩码，用于4KB对齐
 */
#define PAGE_MASK		0xFFFFF000

/*
	获取一个地址的页目录项
 */
#define PGD_INDEX(x)	(((x) >> 22) & 0x3FF)

/*
	获取一个地址的页表项
 */
#define PTE_INDEX(x)	(((x) >> 12) & 0x3FF)

/*
	获取一个地址的页内偏移
 */
#define OFFSET_INDEX(x)	((x) & 0xFFF)

/*
	页目录数据类型
 */
typedef unsigned int pgd_t;

/*
	页表数据类型
 */
typedef unsigned int pte_t;

/*
	页目录成员数
 */
#define PGD_SIZE	(1 << 10)	//10bit

/*
	页表项成员数
 */
#define PTE_SIZE	(1 << 10)

/*
	映射512MB内存需要的页表数
 */
#define PTE_COUNT	128	//*4K*1K

/*
	内核页目录区域
 */
extern pgd_t pgd_kern[PGD_SIZE];

/*
	初始化虚拟内存管理
 */
void init_vmm();

/*
	更换当前的页目录
 */
void switch_pgd(unsigned int pd);

/*
	使用flags指出的页权限，把物理地址pa映射到虚拟地址va
 */
void map(pgd_t * pgd_now, unsigned int va, unsigned int pa, unsigned int flags);

/*
	取消虚拟地址va的物理映射
 */
void unmap(pgd_t *pgd_now, unsigned int va);

/*
	如果虚拟地址va映射到物理地址则返回1
	同时如果pa不是空指针则把物理地址写入pa参数
 */
unsigned int get_mapping(pgd_t *pgd_now, unsigned int va, unsigned int *pa);

/*
	页错误中断的函数处理
 */
void page_fault(pt_regs *regs);

#endif

