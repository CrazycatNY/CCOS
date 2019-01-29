/*
 * ========================================================================================
 *
 *   Filename   :  timer.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月29日11时18分25秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "timer.h"
#include "debug.h"
#include "common.h"
#include "idt.h"


void timer_callback(pt_regs *regs)
{
	static unsigned int tick = 0;
	printk_color(rc_black, rc_red, "Tick: %d\n", tick++);
}

void init_timer(unsigned int frequency)
{
	/*
		注册时间相关的处理函数
	 */
	register_interrupt_handler(IRQ0, timer_callback);
	/*
		Intel 8253/8254 芯片PIT I/O端口地址范围是40h-43h
		输入频率 1193180, frequency即为每秒中断次数
	 */
	unsigned int divisor = 1193180 / frequency;

	/*
		00110110 0x36
		设置8253/8254芯片工作在模式3下
	 */
	outb(0x43, 0x36);

	unsigned char low = (unsigned char)(divisor & 0xFF);
	unsigned char high = (unsigned char )((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
}






