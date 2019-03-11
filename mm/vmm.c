/*
 * ========================================================================================
 *
 *   Filename   :  vmm.c
 *
 *   Description:  
 *
 *   Version    :  1.0
 *   Created    :  2019年01月31日17时27分06秒
 *   Compiler   :  gcc
 *
 *   Author     :  Crazycat(NingYu), ny405888464@gmail.com
 *   Company    :
 *
 * ========================================================================================
 */


#include "idt.h"
#include "string.h"
#include "debug.h"
#include "vmm.h"
#include "pmm.h"

/*
	内核页目录区域
 */
pgd_t pgd_kern[PGD_SIZE] __attribute__((aligned(PAGE_SIZE)));

/*
	内核页表区域
 */
static pte_t pte_kern[PTE_COUNT][PTE_SIZE] __attribute__((aligned(PAGE_SIZE)));

void init_vmm()
{
	/*
		0xC0000000这个地址在页目录的索引
	 */
	unsigned int kern_pte_first_idx = PGD_INDEX(PAGE_OFFSET);

	unsigned int i, j;
	for(i=kern_pte_first_idx, j=0; i<PTE_COUNT + kern_pte_first_idx; i++, j++)
	{
		/*
			此处是内核虚拟地址， MMU需要物理地址， 所以减去偏移，下同
		 */
		pgd_kern[i] = ((unsigned int)pte_kern[j] - PAGE_OFFSET) | PAGE_PRESENT | PAGE_WRITE;
	}

	unsigned int *pte = (unsigned int *)pte_kern;

	/*
		不映射第0页，便于跟踪NULL指针
	 */
	for(i=1; i<PTE_COUNT*PTE_SIZE; i++)
	{
		pte[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	unsigned int pgd_kern_phy_addr = (unsigned int)pgd_kern - PAGE_OFFSET;

	/*
		注册页错误中断的处理函数（14是页故障的中断号）
	 */
	register_interrupt_handler(14, &page_fault);

	switch_pgd(pgd_kern_phy_addr);
}

void switch_pgd(unsigned int pd)
{
	asm volatile("mov %0, %%cr3"::"r"(pd));
}

void map(pgd_t *pgd_now, unsigned int va, unsigned int pa, unsigned int flags)
{
	unsigned int pgd_idx = PGD_INDEX(va);
	unsigned int pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);
	if(!pte)
	{
		pte = (pte_t *)pmm_alloc_page();
		pgd_now[pgd_idx] = (unsigned int)pte | PAGE_PRESENT | PAGE_WRITE;

		/*
			转换到内核线性地址并清0
		 */
		pte = (pte_t *)((unsigned int)pte + PAGE_OFFSET);
		bzero(pte, PAGE_SIZE);
	}
	else
	{
		/*
			转换到内核线性地址
		 */
		pte = (pte_t *)((unsigned int )pte + PAGE_OFFSET);
	}

	pte[pte_idx] = (pa & PAGE_MASK) | flags;

	/*
		通知CPU更新页表缓存
	 */
	asm volatile ("invlpg (%0)"::"a" (va));
}
void unmap(pgd_t *pgd_now, unsigned int va)
{
	unsigned int pgd_idx = PGD_INDEX(va);
	unsigned int pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

	if(!pte)
	{
		return;
	}

	/*
		转换到内核线性地址
	 */
	pte = (pte_t *)((unsigned int)pte + PAGE_OFFSET);
	
	pte[pte_idx] = 0;

	asm volatile("invlpg (%0)" :: "a" (va));
}

unsigned int get_mapping(pgd_t *pgd_now, unsigned int va, unsigned int *pa)
{
	unsigned int pgd_idx = PGD_INDEX(va);
	unsigned int pte_idx = PTE_INDEX(va);

	pte_t *pte = (pte_t *)(pgd_now[pgd_idx] & PAGE_MASK);

	if(!pte)
	{
		return 0;	
	}

	/*
		转换到内核线性地址
	 */
	pte = (pte_t *)((unsigned int)pte + PAGE_OFFSET);

	/*
		如果地址有效且指针不为NULL则返回地址
	 */
	if(pte[pte_idx] != 0 && pa)
	{
		*pa = pte[pte_idx] & PAGE_MASK;
		return 1;
	}

	return 0;
}









