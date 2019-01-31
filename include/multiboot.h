/*
 * ========================================================================================
 *
 *   Filename   :  multiboot.h
 *
 *   Description:  Multiboot结构体定义
 *
 *   Version    :  1.0
 *   Created    :  2019年01月14日09时48分24秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __MULTIBOOT_H
#define __MULTIBOOT_H

#include "types.h"

/*
	启动后，在32位内核进入点，机器状态如下：
		1. CS指向基地址为0x00000000，限长为4G-1的代码段描述符
		2. DS，SS，ES，FS，GS指向基地址为0x00000000，限长为4G-1的数据段描述符
		3. A20地址线已打开
		4. 页机制被禁止
		5. 中断被禁止
		6. EAX = 0x2BADB002
		7. 系统信息和启动信息块的线性地址保存在EBX中（相当于一个指针）
		   以下即为这个信息块的结构
 */

typedef struct multiboot_t{
	unsigned int flags;		//Multiboot的版本信息
	/*
		从BIOS获知的可用内存

		mem_lower和mem_upper分别指出了低端和高端内存的大小，单位是K
		低端内存的首地址是0，高端内存的首地址是1M
		低端内存的最大可能值是640
		高端内存的最大可能值是最大值减去1M，但不保证是这个值
	 */
	unsigned int mem_lower;
	unsigned int mem_upper;

	unsigned int boot_device;	//指出引导程序从哪个bios磁盘设备载入的OS映像
	unsigned int cmdline;		//内核命令行
	unsigned int mods_count;	//boot模块列表
	unsigned int mods_addr;		

	/*
		ELF格式内核映像的section头表、每项的大小、一共有几项以及作为名字索引的字符串表
		它们对应于可执行可链接格式（ELF）的program头中的shdr_*项(shdr_num等)
		所有的section都会被载入，ELF section头的物理地址域指向所有的section在内存中的位置
	 */
	unsigned int num;
	unsigned int size;
	unsigned int addr;
	unsigned int shndx;

	/*
		以下两项指出保存由BIOS提供的内存分布的缓冲区的地址和长度
		mmap_addr是缓冲区的地址，mmap_length是缓冲区的总大小
		缓冲区有一个或者多个下面的结构mmap_entry_t(size实际上是用来跳过下一个对的)组成
	 */
	unsigned int mmap_length;
	unsigned int mmap_addr;

	unsigned int drives_length;//指出第一个驱动器结构的物理地址
	unsigned int drives_addr;	//指出第一个驱动器这个结构的大小
	unsigned int config_table;	//rom配置表
	unsigned int boot_loader_name; //boot loader name
	unsigned int apm_table;		//apm表
	unsigned int vbe_control_info;
	unsigned int vbe_mode_info;
	unsigned int vbe_mode;
	unsigned int vbe_interface_seg;
	unsigned int vbe_interface_off;
	unsigned int vbe_interface_len;
}__attribute__((packed))multiboot_t;

/*
	size是相关结构的大小，单位是字节，他可能大于最小值20
	base_addr_low是启动地址的低32位，base_addr_high是高32位，启动地质总共有64位
	length_low是内存区域大小的低32位，length_high是内存区域大小的高32位，总共是64位
	type是相应地址区间的类型，1代表可用ram，所有其他值代表保留区域
 */
typedef struct mmap_entry_t{
	unsigned int size; //留意size是不包含size自身变量的大小
	unsigned int base_addr_low;
	unsigned int base_addr_high;
	unsigned int length_low;
	unsigned int length_high;
	unsigned int type;
}__attribute__((packed))mmap_entry_t;

/* 声明全局变量 multiboot * 指针 */
extern multiboot_t *mboot_ptr_tmp;
/* 内核页表建立后的指针 */
extern multiboot_t *glb_mboot_ptr;

#endif

