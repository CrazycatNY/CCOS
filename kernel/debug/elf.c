/*
 * ========================================================================================
 *
 *   Filename   :  elf.c
 *
 *   Description:  ELF格式解析相关函数
 *
 *   Version    :  1.0
 *   Created    :  2019年01月14日18时05分03秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */

#include "common.h"
#include "string.h"
#include "elf.h"
#include "vmm.h"

/*
	从multiboot_t结构体获取ELF信息
 */
elf_t elf_from_multiboot(multiboot_t *mb)
{
	int i;
	elf_t elf;
	elf_section_header_t *sh = (elf_section_header_t *)mb->addr;

	unsigned int shstrtab = sh[mb->shndx].addr;
	for(i=0; i<mb->num; i++)
	{
		const char *name = (const char *)(shstrtab + sh[i].name) + PAGE_OFFSET;
		/*
			在GRUB提供的multiboot信息中寻找内核ELF格式所提取的字符串表和符号表
		 */
		if(strcmp(name, ".strtab") == 0)
		{
			elf.strtab = (const char *)sh[i].addr + PAGE_OFFSET;
			elf.strtabsz = sh[i].size;
		}
		if(strcmp(name, ".symtab") == 0)
		{
			elf.symtab = (elf_symbol_t *)(sh[i].addr + PAGE_OFFSET);
			elf.symtabsz = sh[i].size;
		}
	}

	return elf;
}

/* 
	查看ELF的符号信息
 */
const char *elf_lookup_symbol(unsigned int addr, elf_t *elf)
{
	int i;

	for(i=0; i<(elf->symtabsz / sizeof(elf_symbol_t)); i++)
	{
		if(ELF32_ST_TYPE(elf->symtab[i].info) != 0x2)
		{
			continue;
		}
		/*
			通过函数调用地址查到函数的名字（地址在该函数的代码段地址区间内）
		 */
		if((addr >= elf->symtab[i].value) && (addr < (elf->symtab[i].value + elf->symtab[i].size)))
		{
			return (const char *)((unsigned int)elf->strtab + elf->symtab[i].name);
		}
	}

	return NULL;
}





