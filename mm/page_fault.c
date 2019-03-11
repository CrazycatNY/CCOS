/*
 * ========================================================================================
 *
 *   Filename   :  page_fault.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年02月20日18时18分18秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "vmm.h"
#include "debug.h"

void page_fault(pt_regs *regs)
{
	unsigned int cr2;

	asm volatile("mov %%cr2, %0" : "=r"(cr2));

	printk("Page fault at 0x%x, virtual faulting address 0x%x\n", regs->eip, cr2);
	printk("Error code : %x\n", regs->err_code);
	/*
		bit 0 为0值页面不存在
	 */
	if(!(regs->err_code & 0x01))
	{
		printk_color(rc_black, rc_red, "Because the page wasn`t present.\n");
	}
	/*
		bit 1 为0表示读错误，为1表示写错误
	 */
	if(regs->err_code & 0x02)
	{
		printk_color(rc_black, rc_red, "Write error.\n");
	}
	else
	{
		printk_color(rc_black, rc_red, "Read error.\n");
	}

	/*
	   bit 2 为1表示在用户模式打断的，
			为0是在内核模式打断的
	 */
	if(regs->err_code & 0x04)
	{
		printk_color(rc_black, rc_red, "In user mode.\n");
	}
	else
	{
		printk_color(rc_black, rc_red, "In kernel mode.\n");
	}

	/*
	   bit 3 为1表示错误是由保留位覆盖造成的
	 */
	if(regs->err_code & 0x08)
	{
		printk_color(rc_black, rc_red, "Reserved bits being overwritten.\n");
	}

	/*
	   bit 4 为1表示错误发生在取指令的时候
	 */
	if(regs->err_code & 0x10)
	{
		printk_color(rc_black, rc_red, "The fault occurred during an instruction fetch.\n");
	}

	while(1);

}
