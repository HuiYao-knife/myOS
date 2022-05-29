#ifndef __KERNEL_MEMORY_H
#define	__KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"
#include "list.h"
struct virtual_addr {
	struct bitmap vaddr_bitmap;
	uint32_t vaddr_start;
};

enum pool_flags {
	PF_KERNEL = 1,
	PF_USER = 2
};

struct mem_block{
	struct list_elem free_elem;
};

struct mem_block_desc{
	uint32_t block_size;
	uint32_t block_per_arena;
	struct list free_list;
};

struct arena{
	struct mem_block_desc * desc;	
	uint32_t cnt;
	int large;
};

#define	DESC_CNT	7
#define	PG_P_1	1
#define	PG_P_0	0
#define	PG_RW_R	0
#define	PG_RW_W	2
#define	PG_US_S	0
#define	PG_US_U	4

extern struct pool kernel_pool, user_pool;
void mem_init(void);
static void * vaddr_get(enum pool_flags pf, uint32_t pg_cnt);
uint32_t * pte_ptr(uint32_t vaddr);
static void * palloc(struct pool * m_pool);
uint32_t * pde_ptr(uint32_t vaddr);

static void page_table_add(void * _vaddr, void * _page_phyaddr);
void * get_kernel_pages(uint32_t pg_cnt);
void * malloc_page(enum pool_flags pf, uint32_t pg_cnt);
void block_desc_init(struct mem_block_desc * desc_array);
struct mem_block* arena2block(struct arena * a, uint32_t idx);
struct arena * block2arena(struct mem_block * b);
void * sys_malloc(uint32_t size);
void pfree(uint32_t pg_phy_addr);
static void page_table_pte_remove(uint32_t vaddr);
static void vaddr_remove(enum pool_flags pf, void * _vaddr, uint32_t pg_cnt);
void sys_free(void * ptr);
void mfree_page(enum pool_flags pf, void * _vaddr, uint32_t pg_cnt);
void * get_a_page_without_opvaddrbitmap(enum pool_flags pf, uint32_t vaddr);
void free_a_phy_page(uint32_t pg_phy_addr);
#endif
