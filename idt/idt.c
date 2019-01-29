/*
 * ========================================================================================
 *
 *   Filename   :  idt.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月23日15时00分48秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */

#include "common.h"
#include "string.h"
#include "debug.h"
#include "idt.h"

/*
	中断描述符表
 */
idt_entry_t idt_entries[256];

/*
	IDTR
 */
idt_ptr_t idt_ptr;

/*
	中断处理函数的指针数组
 */
interrupt_handler_t interrupt_handlers[256];

/*
	设置中断描述符
 */
static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);

/*
	声明加载IDTR的函数
 */
extern void idt_flush(unsigned int);

/*
	初始化中断描述符表
 */
void init_idt()
{
	/*
		重新映射IRQ表
		两片级联的Intel 8259A芯片
		主片端口0x20 0x21
		从片端口0xA0 0xA1
	 */

	/*
		初始化主片、从片
		00010001
	 */
	outb(0x20, 0x11);
	outb(0xA0, 0x11);

	/*
		设置主片irq从0x20（32）号中断开始
			从片irq从0x28（40）号中断开始
	 */
	outb(0x21, 0x20);
	outb(0xA1, 0x28);

	/* 
		设置主片ir2引脚链接从片
	 */
	outb(0x21, 0x04);

	/*
		告诉从片输出引脚和主片ir2号相连
	 */
	outb(0xA1, 0x02);

	/*
		设置主片和从片按照8086的工作方式
	 */
	outb(0x21, 0x01);
	outb(0xA1, 0x01);

	/*
		设置主从片允许中断
	 */
	outb(0x21, 0x0);
	outb(0xA1, 0x0);


	bzero((unsigned char *)&interrupt_handlers, sizeof(interrupt_handler_t) * 256);

	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt_entries;

	bzero((unsigned char *)&idt_entries, sizeof(idt_entry_t) * 256);

	/* 0-32: 用于CPU的中断处理 */
	idt_set_gate(0, (unsigned int)isr0, 0x08, 0x8E);
	idt_set_gate(1, (unsigned int)isr1, 0x08, 0x8E);
	idt_set_gate(2, (unsigned int)isr2, 0x08, 0x8E);
	idt_set_gate(3, (unsigned int)isr3, 0x08, 0x8E);
	idt_set_gate(4, (unsigned int)isr4, 0x08, 0x8E);
	idt_set_gate(5, (unsigned int)isr5, 0x08, 0x8E);
	idt_set_gate(6, (unsigned int)isr6, 0x08, 0x8E);
	idt_set_gate(7, (unsigned int)isr7, 0x08, 0x8E);
	idt_set_gate(8, (unsigned int)isr8, 0x08, 0x8E);
	idt_set_gate(9, (unsigned int)isr9, 0x08, 0x8E);
	idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8E);
	idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8E);
	idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8E);
	idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8E);
	idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8E);
	idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8E);
	idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8E);
	idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8E);
	idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8E);
	idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8E);
	idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8E);
	idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8E);
	idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8E);
	idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8E);
	idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8E);
	idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8E);
	idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8E);
	idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8E);
	idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8E);
	idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8E);
	idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8E);
	idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8E);

	idt_set_gate(32, (unsigned int)irq0, 0x08, 0x8E);
	idt_set_gate(33, (unsigned int)irq1, 0x08, 0x8E);
	idt_set_gate(34, (unsigned int)irq2, 0x08, 0x8E);
	idt_set_gate(35, (unsigned int)irq3, 0x08, 0x8E);
	idt_set_gate(36, (unsigned int)irq4, 0x08, 0x8E);
	idt_set_gate(37, (unsigned int)irq5, 0x08, 0x8E);
	idt_set_gate(38, (unsigned int)irq6, 0x08, 0x8E);
	idt_set_gate(39, (unsigned int)irq7, 0x08, 0x8E);
	idt_set_gate(40, (unsigned int)irq8, 0x08, 0x8E);
	idt_set_gate(41, (unsigned int)irq9, 0x08, 0x8E);
	idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8E);
	idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8E);
	idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8E);
	idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8E);
	idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8E);
	idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8E);

	/*                                
		255用来实现系统调用
	 */
	idt_set_gate(255, (unsigned int)isr255, 0x08, 0x8E);

	/*
		更新设置中断描述符表
	 */
	idt_flush((unsigned int)&idt_ptr);
}

/*
	设置中断描述符
 */
static void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
	
	idt_entries[num].sel = sel;
	idt_entries[num].always0 = 0;


	/*
	   先留下0x60这个魔数，以后实现用户态的时候，这个运算可以设置中断门的特权级别为3
	*/
	idt_entries[num].flags = flags;		// | 0x60
}

/*
	调用中断处理函数
 */
void isr_handler(pt_regs *regs)
{
	if(interrupt_handlers[regs->int_no])
	{
		interrupt_handlers[regs->int_no](regs);
	}
	else
	{
		printk_color(rc_black, rc_white, "Unhandled interrupt:%d\n", regs->int_no);
	}
}

void register_interrupt_handler(unsigned char n, interrupt_handler_t h)
{
	interrupt_handlers[n] = h;
}

/*
	IRQ处理函数
 */
void irq_handler(pt_regs *regs)
{
	/*
		发送中断信号给PICs
		从32号中断开始，Intel 8259A只能处理8级中断，
		故大于40的中断信号都有从片处理
	 */
	if(regs->int_no >= 40)
	{
		/* 发送重设信号给从片 */	
		outb(0xA0, 0x20);
	}
	/* 发送重设信号给主片 */
	outb(0x20, 0x20);

	if(interrupt_handlers[regs->int_no])
	{
		interrupt_handlers[regs->int_no](regs);
	}
}




