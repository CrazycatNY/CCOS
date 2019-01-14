/*
 * ========================================================================================
 *
 *   Filename   :  elf.h
 *
 *   Description:  ELF格式的部分定义
 *
 *   Version    :  1.0
 *   Created    :  2019年01月11日18时37分11秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#ifndef __ELF_H
#define __ELF_H

#include "types.h"
#include "multiboot.h"

#define ELF32_ST_TYPE(i) ((i)&0xf)

/* ELF 格式区段头 */
typedef struct elf_section_header_t{
	unsigned int name;
	unsigned int type;
	unsigned int flags;
	unsigned int addr;
	unsigned int offset;
	unsigned int size;
	unsigned int link;
	unsigned int info;
	unsigned int addralign;
	unsigned int entsize;
}__attribute__((packed))elf_section_header_t;

/* ELF 格式符号 */
typedef struct elf_symbol_t{
	unsigned int name;
	unsigned int value;
	unsigned int size;
	unsigned int info;
	unsigned int other;
	unsigned short shndx;
}__attribute__((packed))elf_symbol_t;

/* ELF信息 */
typedef struct elf_t{
	elf_symbol_t *symtab;
	unsigned int symtabsz;
	const char *strtab;
	unsigned int strtabsz;
}elf_t;

/* 从multiboot_t结构获取ELF信息 */
elf_t elf_from_multiboot(multiboot_t *mb);

/* 查看ELF的符号信息 */
const char *elf_lookup_symbol(unsigned int addr, elf_t *elf);

#endif

