;-----------------------------------------
;
;	boot.s -- 内核从这里开始
;
;-----------------------------------------

;Multiboot 魔数，有规范规定
MBOOT_HEADER_MAGIC	equ	0x1BADB002

;bit 0表示所有引导模块按页(4KB)边界对齐
MBOOT_PAGE_ALIGN	equ	1 << 0

;bit 1告诉grub把内存信息包含在Multiboot信息结构中
MBOOT_MEM_INFO		equ 1 << 1

;定义使用的Multiboot标记
MBOOT_HEADER_FLAGS	equ	MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

;域checksum是一个32位的无符号值，当与其他magic域（也就是magic和flags）相加
;时，要求其结果必须时32位的无符号0（magic+flags+checksum=0）
MBOOT_CHECKSUM		equ	-(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

;符合Multiboot规范的OS映像需要这样一个magic Multiboot头，
;Multiboot头的分布必须如下表所示：
;---------------------------------------------
; 偏移量	类型	域名		备注
;
;   0		u32		magic		必需
;	4		u32		flags		必需
;	8		u32		checksum	必需
;
;	这些就够了，剩下的得去看gnu文档
;----------------------------------------------

[BITS 32]		;所有代码以32-bit的方式编译
section .text	;代码段开始

;在代码段的起始位置设置符合Multiboot规范的标记

dd MBOOT_HEADER_MAGIC	;GRUB会通过这个魔数判断该映像是否支持
dd MBOOT_HEADER_FLAGS	;GRUB的一些加载选项
dd MBOOT_CHECKSUM		;检测数值，其含义在定义处

[GLOBAL start]			;向外部声明内核代码入口，此处提供该声明给链接器
[GLOBAL glb_mboot_ptr]	;向外部声明struct multiboot *变量
[EXTERN kern_entry]	;声明内核c代码的入口函数

start:
	cli					;此时还没有设置好保护模式的中断处理，要关闭中断
						;所以必须关闭中断
	mov esp, STACK_TOP	;设置内核栈地址
	and ebp, 0			;栈指针修改为0
	and esp, 0FFFFFFF0H ;栈地址按16字节对齐
	mov [glb_mboot_ptr], eax	;将ebx中的值存入全局变量
	call kern_entry		;调用内核入口函数
stop:
	hlt					;停机指令，降低cpu功耗
	jmp stop			;到这里结束，关机啥的后边再说

;-------------------------------------------------------------------

section .bss			;未初始化的数据段开始
stack:
	resb 32768			;内核栈
glb_mboot_ptr:
	resb 4				;全局的Multiboot结构体指针

STACK_TOP equ $-stack-1	;内核栈顶，$表示当前地址
