/*
 * ========================================================================================
 *
 *   Filename   :  gdt.c
 *
 *   Description:  全局描述符表相关函数
 *
 *   Version    :  1.0
 *   Created    :  2019年01月22日10时29分37秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "gdt.h"
#include "string.h"

/*
	全局描述符表长度
 */
#define GDT_LENGTH 5

/*
	全局描述符表定义
 */
gdt_entry_t gdt_entries[GDT_LENGTH];

/* GDTR */
gdt_ptr_t gdt_ptr;

/* 声明内核栈地址 */
extern unsigned int stack;

/*
	全局描述符表构造函数

    structure of GDT entries:
    +----------------------------+--------------------------+
    |31                        16|15                       0|
    +----------------------------+--------------------------+
    |         Base 0:15          |       Limit 0:15         |
    +------------+-------+-------+-------------+------------+
    |63        56|55   52|51   48|47         40|39        32|
    +------------+-------+-------+-------------+------------+
    | Base 24:31 | Flags | Limit | Access Byte | Base 16:23 |
    +------------+-------+-------+-------------+------------+
    
    structure of access byte and flags:
    Access Byte:                                Flags:
    +------------------------------------+      +-----------------+
    |7                                  0|      |7               4|
    +----+-------+---+----+----+----+----+      +----+----+---+---+
    | pr | privl | s | Ex | DC | RW | AC |      | Gr | Sz | 0 | 0 |
    +----+-------+---+----+----+----+----+      +----+----+---+---+
    
    the bit fields see https://wiki.osdev.org/GDT
 */
static void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned gran)
{
	gdt_entries[num].base_low = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_low = (base >> 24) & 0xFF;
			
	gdt_entries[num].limit_low = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= (gran & 0x0F);
	gdt_entries[num].access = access;
}

void init_gdt()
{
	/*
		全局描述符表界限从0开始，总长度-1
	 */
	gdt_ptr.limit = sizeof(gdt_entry_t) * GDT_LENGTH - 1;
	gdt_ptr.base = (unsigned int)&gdt_entries;

	/*
		采用Intel平坦模式
	 */
	gdt_set_gate(0, 0, 0, 0, 0);	//按照Intel文档要求，第一个描述符必须全0
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //指令段
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); //数据段
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //用户模式代码段
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); //用户模式数据段

	/*
		加载全局描述符表地址到gdtr寄存器
	 */
	gdt_flush((unsigned int )&gdt_ptr);
}




