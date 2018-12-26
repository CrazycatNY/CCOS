#ifndef __INCLUDE_CONSOLE_H
#define __INCLUDE_CONSOLE_H

typedef enum{
	rc_black,
	rc_blue,
	rc_green,
	rc_cyan,
	rc_red,
	rc_magenta = 5,
	rc_brown,
	rc_light_grey,
	rc_dark_grey,
	rc_light_blue,
	rc_light_green = 10,
	rc_light_cyan,
	rc_light_red,
	rc_light_magenta,
	rc_light_brown,
	rc_white = 15
}real_color_t;

/*
	清屏操作
 */
void console_clear();

/*
	输出单个字符 带颜色
 */
void console_putc_color(char c, real_color_t back, real_color_t fore);

/*
	打印一个以\0结尾的字符串 默认黑底白字
 */
void console_write(char *cstr);

/*
	打印一个以\0结尾的字符串 带颜色
 */
void console_write_color(char *cstr, real_color_t back, real_color_t fore);

/*
	屏幕输出一个十六进制的整型数
 */
void console_write_hex(unsigned int n, real_color_t back, real_color_t fore);

/*
	屏幕输出一个十进制的整型数
 */
void console_write_dec(unsigned int n, real_color_t back, real_color_t fore);

#endif //__INCLUDE_CONSOLE_H
