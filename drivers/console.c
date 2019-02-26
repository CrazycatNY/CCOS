#include "console.h"
#include "common.h"
#include "vmm.h"

/* 
	VGA 显示缓冲的起点是 0xB8000
 */
static unsigned short *video_memory = (unsigned short *)(0xB8000 + PAGE_OFFSET);

/*
	光标的横纵坐标
 */
static unsigned char cursor_x = 0;
static unsigned char cursor_y = 0;

/*
	显示光标
 */
static void move_cursor()
{
	/*
		屏幕宽度80字节
	 */
	unsigned short cursor_location = cursor_y * 80 + cursor_x;

	/*
		两个内部寄存器14,15分别表示光标的位置的高8位与低8位
	 */
	outb(0x3D4, 14); //0x3D4为设备内部寄存器索引
	outb(0x3D5, cursor_location >> 8);//0x3D5用来设置上边索引好的寄存器
	outb(0x3D4, 15);
	outb(0x3D5, cursor_location);
}

/*
	清屏, 使用白底黑字的空格覆盖整个屏幕
 */
void console_clear()
{
	unsigned char attribute_byte = (0 << 4) | (15 & 0x0F);
	unsigned short blank = 0x20 | (attribute_byte << 8);

	int i;
	for(i=0; i<80*25; i++)
	{
		video_memory[i] = blank;
	}
	
	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

/*
	屏幕滚动，后24行向上挪动1行，最后一行清空
 */
static void scroll()
{
	/*
		构造一个黑底白字的描述
	 */
	unsigned char attribute_byte = (0 << 4) | (15 & 0x0F);
	unsigned short blank = 0x20 | (attribute_byte << 8);

	if(cursor_y >= 25)
	{
		int i;
		for(i=0; i<24*80; i++)
		{
			video_memory[i] = video_memory[i+80];
		}

		/*
			最后一行填充空格
		 */
		for(i=24*80; i<25*80; i++)
		{
			video_memory[i] = blank;
		}
		
		/*
			上移了一行
		 */
		cursor_y = 24;
	}
}

/*
	显示字符,带颜色
 */
void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	unsigned char back_color = (unsigned char )back;
	unsigned char fore_color = (unsigned char )fore;

	unsigned char attribute_byte = (back_color << 4) | (fore_color & 0x0F);
	unsigned short attribute = attribute_byte << 8;

	if(c == 0x08 && (cursor_x!=0))
	{//退格键
		cursor_x--;
	}
	else if(c == 0x09)
	{//tab
		cursor_x = (cursor_x+8) & ~(8-1);
	}
	else if(c == '\r')
	{
		cursor_x = 0;
	}
	else if(c == '\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if(c >= ' ')
	{
		video_memory[cursor_y * 80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	/*
		满80个字符换行
	 */
	if(cursor_x >= 80)
	{
		cursor_x = 0;
		cursor_y++;
	}

	/*
		滚屏
	 */
	scroll();

	/*
		移动光标
	 */
	move_cursor();
}

/*
	输出黑底白字字符串
 */
void console_write(char *cstr)
{
	while(*cstr)
	{
		console_putc_color(*cstr++, rc_black, rc_white);
	}
}

/*
	输出指定颜色的字符串
 */
void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while(*cstr)
	{
		console_putc_color(*cstr++, back, fore);
	}
}


