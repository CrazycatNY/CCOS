#include "console.h"
#include "debug.h"

int kern_entry()
{
	init_debug();

	init_gdt();

	init_idt();

	console_clear();

	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");

	panic("test");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");
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
