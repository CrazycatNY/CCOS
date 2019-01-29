#include "console.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "pmm.h"

int kern_entry()
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
	
	printk_color(rc_black, rc_red, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

	unsigned int alloc_addr = NULL;
	printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc:\n");

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);

	alloc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", alloc_addr);
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
	return 0;
}
