/*
 * ========================================================================================
 *
 *   Filename   :  heap.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年03月04日15时51分40秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "heap.h"
#include "debug.h"
#include "pmm.h"
#include "vmm.h"

static header_t *heap_first;

static unsigned int heap_max = HEAP_START;

void init_heap()
{
	heap_first = 0;
}
static void alloc_chunk(header_t *start, unsigned int len)
{
	while(((unsigned int)start + len) > heap_max)
	{
		unsigned int page = pmm_alloc_page();
		map(pgd_kern, heap_max, page, PAGE_PRESENT | PAGE_WRITE);
		heap_max += PAGE_SIZE;
	}
}
/*
	分配的内存在中间，并且长度大于所需的情况下分割
 */
static void split_chunk(header_t *start, unsigned int len)
{
	if(start->len - len > sizeof(header_t))
	{
		header_t *new_chunk = (void *)start + len;
		
		new_chunk->prev = start;
		new_chunk->next = start->next;
		start->next = new_chunk;
		new_chunk->next->prev = new_chunk;
		
		new_chunk->allocated = 0;
		new_chunk->len = start->len - len;
		start->len = len;
	}
}
void *kmalloc(unsigned int len)
{
	len += sizeof(header_t);

	header_t *cur_header = heap_first;
	header_t *prev_header = NULL;

	while(cur_header)
	{
		if((cur_header->allocated == 0) && (cur_header->len >= len))
		{
			/*
				切割内存
			 */
			split_chunk(cur_header, len);	
			//分配
			cur_header->allocated = 1;
			return ((void*)cur_header + sizeof(header_t));
		}
		//不满足条件，看下一个
		prev_header = cur_header;
		cur_header = cur_header->next;
	}

	if(prev_header)
	{
		//加在prev后面
		cur_header = ((void *)prev_header + prev_header->len);
		prev_header->next = cur_header;
	}
	else
	{
		//第一次
		heap_first = (header_t *)HEAP_START;
		cur_header = (header_t *)HEAP_START;
	}
	
	alloc_chunk(cur_header, len);
	cur_header->prev = prev_header;
	cur_header->next = NULL;
	cur_header->allocated = 1;
	cur_header->len = len;
	
	return ((void *)cur_header + sizeof(header_t));
}

static void free_chunk(header_t *start)
{
	if(start->prev == NULL)
	{
		heap_first = 0;
	}
	else
	{
		start->prev->next = NULL;
	}

	while((heap_max - (unsigned int)start) >= PAGE_SIZE)
	{
		heap_max -= PAGE_SIZE;
		unsigned int page;
		get_mapping(pgd_kern, heap_max, &page);
		unmap(pgd_kern, heap_max);
		pmm_free_page(page);
	}
}

static void glue_chunk(header_t *cur_header)
{
	if((cur_header->prev != NULL) && cur_header->prev->allocated == 0)
	{
		cur_header->prev->next = cur_header->next;
		if(cur_header->next)
			cur_header->next->prev = cur_header->prev;

		cur_header->prev->len += cur_header->len;
		cur_header = cur_header->prev;
	}

	if((cur_header->next != NULL) && (cur_header->next->allocated == 0))
	{
		cur_header->len += cur_header->next->len;

		cur_header->next = cur_header->next->next;
		if(cur_header->next->next)
			cur_header->next->next->prev = cur_header->prev;
	}

	if(cur_header->next == NULL)
	{
		//直接释放
		free_chunk(cur_header);
	}
}
void kfree(void *start)
{
	header_t *header = (void *)start - sizeof(header_t);

	header->allocated = 0;
	glue_chunk(header);
}

void test_heap()
{
	printk_color(rc_black, rc_magenta, "Test kmalloc() && kfree() now ...\n");
	void *addr1 = kmalloc(50);
	printk("kmalloc 50 byte in 0x%X\n", addr1);
	void *addr2 = kmalloc(500);
	printk("kmalloc 500 byte in 0x%X\n", addr2);
	void *addr3 = kmalloc(5000);
	printk("kmalloc 5000 byte in 0x%X\n", addr3);
	void *addr4 = kmalloc(50000);
	printk("kmalloc 50000 byte in 0x%X\n", addr4);

	printk("free mem in 0x%X\n", addr1);
	kfree(addr1);
	printk("free mem in 0x%X\n", addr2);
	kfree(addr2);
	void *addr5 = kmalloc(60);
	printk("kmalloc 60 byte in 0x%X\n", addr5);
	
	printk("free mem in 0x%X\n", addr5);
	kfree(addr5);
	printk("free mem in 0x%X\n", addr3);
	kfree(addr3);
	printk("free mem in 0x%X\n", addr4);
	kfree(addr4);
}



