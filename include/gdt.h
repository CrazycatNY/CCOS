/*
 * ========================================================================================
 *
 *   Filename   :  gdt.h
 *
 *   Description:  GDT描述符定义
 *
 *   Version    :  1.0
 *   Created    :  2019年01月22日10时03分32秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __GDT_H
#define __GDT_H

/*
	全局描述符类型
 */
typedef struct gdt_entry_t{
	unsigned short limit_low;	/* 段界限15-0 */
	unsigned short base_low;	/* 段基地址15-0 */
	unsigned char base_middle;	/* 段基地址23-16 */
	unsigned char access;		/* 段存在位、描述符特权级、描述符类型、描述符子类型 */
	unsigned char granularity;	/* 其他标志、段界限19-16 */
	unsigned char base_high;	/* 段基地址31-24 */
}__attribute__((packed))gdt_entry_t;

/*
	GDTR
 */
typedef struct gdt_ptr_t{
	unsigned short limit;	/* 全局描述符表限长 */
	unsigned int base;		/* 全局描述符表32位基址 */
}__attribute__((packed))gdt_ptr_t;

/*
	初始化全局描述符表
 */
void init_gdt();

/*
	GDT加载到GDTR的函数[汇编实现]
 */
extern void gdt_flush(unsigned int);

#endif

