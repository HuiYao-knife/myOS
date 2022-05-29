#ifndef __INTERRUPT_H
#define	__INTERRUPT_H
#include "stdint.h"

struct gate_desc {
	uint16_t	func_offset_low_word;
	uint16_t	selector;
	uint8_t		dcount;
	uint8_t		attribute;
	uint16_t	func_offset_high_word;
};

enum intr_status intr_enable();
enum intr_status{
	INTR_OFF,
	INTR_ON
};
#define	IDT_DESC_CNT	0x81	
#define	PIC_M_CTRL	0x20
#define	PIC_M_DATA	0x21
#define	PIC_S_CTRL	0xa0
#define	PIC_S_DATA	0xa1
#define	EFLAGS_IF	0x00000200
#define	GET_EFLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0":"=g"(EFLAG_VAR))
void register_handler(uint8_t vector_no, void * function);
static void general_intr_handler(uint8_t num);
static void exception_init(void);
static void make_idt_desc(struct gate_desc * p_gdesc, uint8_t attr, void * function);
static void idt_desc_init(void);
void idt_init();
static void pic_init(void);
extern uint32_t syscall_handler(void);
extern void * intr_entry_table[IDT_DESC_CNT];

enum intr_status intr_get_status();
enum intr_status intr_set_status(enum intr_status status);
enum intr_status intr_disable();
#endif
