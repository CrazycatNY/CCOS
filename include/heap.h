/*
 * ========================================================================================
 *
 *   Filename   :  heap.h
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年03月04日15时49分31秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __HEAP_H
#define __HEAP_H

#define HEAP_START 0xE0000000;
/*
	内存管理结构
 */
typedef struct header{
	struct header *prev;
	struct header *next;
	unsigned int allocated:1;
	unsigned int len:31;
}header_t;

void init_heap();

void *kmalloc(unsigned int len);

void kfree(void *p);

void test_heap();
#endif

