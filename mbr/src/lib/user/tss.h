#ifndef	__TSS_H
#define	__TSS_H
#include "stdint.h"
#include "thread.h"
#include "global.h"

struct gdt_desc{
	uint16_t limit_low_word;
	uint16_t base_low_word;
	uint8_t	base_mid_byte;
	uint8_t	attr_low_byte;
	uint8_t limit_high_attr_high;
	uint8_t base_high_byte;
};
void update_tss_esp0(struct task_struct * pthread);
void tss_init();
static struct gdt_desc make_gdt_desc(uint32_t * desc_addr, uint32_t limit, uint8_t attr_low, uint8_t attr_high);
#endif
