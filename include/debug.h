/*
 * ========================================================================================
 *
 *   Filename   :  debug.h
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月11日18时06分48秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __DEBUG_H
#define __DEBUG_H

#include "console.h"
#include "vargs.h"
#include "elf.h"

#define assert(x, info) \
	do{\
		if(!(x))\
		{\
			panic(info);\
		}\
	}while(0)

/* 编译期间静态检测 */
#define static_assert(x)\
	switch (x) {case 0: case (x): ;}

/* 初始化 Debug 信息 */
void init_debug(void);

/* 打印当前的函数调用栈信息 */
void panic(const char *msg);

/* 打印当前的段寄存器值 */
void print_cur_status(void);

/* 内核的打印函数 */
void printk(const char *format, ...);

/* 内核带颜色打印 */
void printk_color(real_color_t back, real_color_t fore, const char *format, ...);

#endif

