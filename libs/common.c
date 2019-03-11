#include "common.h"
/*
	端口写一个字节
 */
inline void outb(unsigned short port, unsigned char value)
{
	asm volatile("outb %1, %0" :: "dN"(port), "a"(value));
}

/*
   端口读一个字节
 */
inline unsigned char inb(unsigned short port)
{
	unsigned char ret;

	asm volatile("inb %1, %0" : "=a"(ret):"dN"(port));

	return ret;
}

/*
	端口读一个字
 */
inline unsigned short inw(unsigned short port)	
{
	unsigned short ret;

	asm volatile("inw %1, %0" : "=a"(ret):"dN"(port));

	return ret;
}

/*
	开启中断
 */
//inline void enable_intr()
//{
//	asm volatile("sti");
//}
//
///*
//	断开中断
// */
//inline void disable_intr()
//{
//	asm volatile("cli" ::: "memory");
//}


