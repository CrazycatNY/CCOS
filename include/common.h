#ifndef __INCLUDE_COMMON_H
#define __INCLUDE_COMMON_H

/*
	端口写一个字节
 */
void outb(unsigned short port, unsigned char value);

/*
	端口读一个字节
 */
unsigned char inb(unsigned short port);

/*
	端口读一个字
 */
unsigned short inw(unsigned short port);

#endif //__INCLUDE_COMMON_H












