/*
 * ========================================================================================
 *
 *   Filename   :  vargs.h
 *
 *   Description:  可变形参表相关宏定义
 *
 *   Version    :  1.0
 *   Created    :  2019年01月10日17时33分34秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __VARGS_H
#define __VARGS_H

typedef __builtin_va_list va_list;

#define va_start(ap, last)  (__builtin_va_start(ap, last))
#define va_arg(ap, type)	(__builtin_va_arg(ap, last))
#define va_end(ap)

#endif

