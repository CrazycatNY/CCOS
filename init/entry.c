#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"
#include "vmm.h"
#include "heap.h"

/*
	内核初始化函数
 */
void kern_init();

/*
	开启分页机制之后的Multiboot数据指针
 */
multiboot_t *glb_mboot_ptr;

/*
	开启分页之后的内核栈
 */
char kern_stack[STACK_SIZE];

/*
	内核使用的临时页表和页目录
	该地址必须是页对齐的地址，内存0-640KB肯定是空闲的
 */
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

/*
	内核入口函数
 */
__attribute__((section(".init.text"))) void kern_entry()
{
	pgd_tmp[0] = (unsigned int)pte_low | PAGE_PRESENT | PAGE_WRITE;
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (unsigned int)pte_high | PAGE_PRESENT | PAGE_WRITE;

	/*
		映射内核虚拟地址4MB到物理地址的前4MB
	 */
	int i;
	for(i=0; i<1024; i++)
	{
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	/*
		映射0x00000000-0x00400000的物理地址到虚拟地址的0xC0000000-0xC0400000
	 */
	for(i=0; i<1024; i++)
	{
		pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	/*
		设置临时页表
	 */
	asm volatile("mov %0, %%cr3"::"r" (pgd_tmp));

	unsigned int cr0;

	/*
		启用分页，将cr0寄存器的分页位置1就好
	 */
	asm volatile("mov %%cr0, %0":"=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0"::"r"(cr0));

	/*
		切换内核栈
	 */
	unsigned int kern_stack_top = ((unsigned int)kern_stack + STACK_SIZE) & 0xFFFFFFF0;
	asm volatile ("mov %0, %%esp\n\t"
					"xor %%ebp, %%ebp"::"r"(kern_stack_top));

	/*
		更新全局Multiboo_t指针
	 */
	glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

	/*
		调用内核初始化函数
	 */
	kern_init();
}

void kern_init()
{
	init_debug();

	init_gdt();

	init_idt();

	console_clear();

	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");

	init_timer(200);

	//	asm volatile ("sti");

	printk("kernel in memory start: 0x%08X\n", kern_start);
	printk("kernel in memory end:	0x%08X\n", kern_end);
	printk("kernel in memory used:	0x%d KB\n\n", (kern_end - kern_start + 1023) / 1024);
	
	show_memory_map();

	init_pmm();
	init_vmm();
	init_heap();
	
	printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

	unsigned int alloc_addr = NULL;
	printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc:\n");

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	printk_color(rc_black, rc_light_brown, "Free 0x%0X\n", alloc_addr);
	pmm_free_page(alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	printk_color(rc_black, rc_light_brown, "Free 0x%0X\n", alloc_addr);
	pmm_free_page(alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	printk_color(rc_black, rc_light_brown, "Free 0x%0X\n", alloc_addr);
	pmm_free_page(alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);
	
	printk_color(rc_black, rc_light_brown, "Free 0x%0X\n", alloc_addr);
	pmm_free_page(alloc_addr);

	test_heap();
	//panic("test");

	//asm volatile ("int $0x3");
	//asm volatile ("int $0x4");
	//console_write_color("Hello, OS kernel!\n", rc_black, rc_green);
	//unsigned char *input = (unsigned char *)0xB8000;
	//unsigned char color = (0 << 4) | (15 & 0x0F);

	//*input++ = 'H'; *input++ = color;
	//*input++ = 'e'; *input++ = color;
	//*input++ = 'l'; *input++ = color;
	//*input++ = 'l'; *input++ = color;
	//*input++ = 'o'; *input++ = color;
	//*input++ = ','; *input++ = color;
	//*input++ = ' '; *input++ = color;
	//*input++ = 'k'; *input++ = color;
	//*input++ = 'e'; *input++ = color;
	//*input++ = 'r'; *input++ = color;
	//*input++ = 'n'; *input++ = color;
	//*input++ = 'e'; *input++ = color;
	//*input++ = 'l'; *input++ = color;
	//*input++ = '!'; *input++ = color;
	while(1)
	{
		asm volatile("hlt");
	}
}
