#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "debug.h"
#include "io.h"



extern uint32_t syscall_handler(void);
extern void set_cursor(uint16_t num);
static struct gate_desc idt[IDT_DESC_CNT];
char * intr_name[IDT_DESC_CNT];
void * idt_table[IDT_DESC_CNT];
extern void * intr_entry_table[IDT_DESC_CNT];
void register_handler(uint8_t vector_no, void * function){
	idt_table[vector_no] = function;
}

static void general_intr_handler(uint8_t num)
{
	if(num == 0x27 || num == 0x2f){
		return;
	}
	asm("movl $0,%ebx");
	gset_cursor(0);
	int cursor_pos = 0;
	while(cursor_pos < 320){
		put_char(' ');
		cursor_pos++;
	}
	asm("movl $0,%ebx");
	gset_cursor(0);
	put_str("excetion massage begin!\n");

	asm("movl $88,%ebx");
	gset_cursor(88);
	put_str(intr_name[num]);
	if(num == 14){
		int page_fault_vaddr = 0;	
		asm("movl %%cr2, %0" : "=r" (page_fault_vaddr));
		put_str("\npage fault addr is");
		put_int(page_fault_vaddr);
	}
	put_str("\nexcetion message end!\n");
	while(1);
}

static void exception_init(void)
{
	int i;
	for(i = 0; i < IDT_DESC_CNT; i++){
		idt_table[i] = general_intr_handler;
		intr_name[i] = "unknown";
	}
	intr_name[0] = "#DE Divide Error";
	intr_name[1] = "#DB Debug Exception";
	intr_name[2] = "#NMI Interrupt";
	intr_name[3] = "#BP Breakpoint Exception";
	intr_name[4] = "#OF Overflow Exception";
	intr_name[5] = "#BR BOUND Range Exceeded Exception";
	intr_name[6] = "#UD Invalid Opcode Exception";
	intr_name[7] = "#NM Device Not Available Exception";
	intr_name[8] = "#DF Double Fault Exception";
	intr_name[9] = "#Coprocessor Segment Overrun";
	intr_name[10] = "#TS Invalid TSS Exception";
	intr_name[11] = "#NP Segment Not Present";
	intr_name[12] = "#SS Stack Fault Exception";
	intr_name[13] = "#GP General Protection Exception";
	intr_name[14] = "#PF Page-Fault Exception";
	intr_name[16] = "#MF x87 FPU Floating-Point Error";
	intr_name[17] = "#AC Alignment Check Exception";
	intr_name[18] = "#MC Machine-Check Exception";
	intr_name[19] = "#XF SIMD Floaing-Point Exception";
}


static void make_idt_desc(struct gate_desc * p_gdesc, uint8_t attr, void * function)
{
	p_gdesc->func_offset_low_word = ((uint32_t)function & 0x0000ffff);
	p_gdesc->selector = SELECTOR_K_CODE;
	p_gdesc->dcount = 0;
	p_gdesc->attribute = attr;
	p_gdesc->func_offset_high_word = ((uint32_t)function & 0xffff0000) >> 16;
}

static void idt_desc_init(void)
{
	int i;
	for(i = 0; i < IDT_DESC_CNT; i++){
		make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
	}
	make_idt_desc(&idt[0x80], IDT_DESC_ATTR_DPL3, syscall_handler);
	put_str("idt desc init done!\n");
}

static void pic_init(void)
{
	
	outb(PIC_M_CTRL, 0x11);
	outb(PIC_M_DATA, 0x20);

	outb(PIC_M_DATA, 0x04);
	outb(PIC_M_DATA, 0x01);

	outb(PIC_S_CTRL, 0x11);
	outb(PIC_S_DATA, 0x28);

	outb(PIC_S_DATA, 0x02);
	outb(PIC_S_DATA, 0x01);

	outb(PIC_M_DATA, 0xf8);
	outb(PIC_S_DATA, 0xbf);
	//outb(PIC_M_DATA, 0xfe);//only open time interrupt
	//outb(PIC_S_DATA, 0xff);

	//outb(PIC_M_DATA, 0xfd);  //only open keyboard interrupt
	//utb(PIC_S_DATA, 0xff);
	//outb(PIC_M_DATA, 0xfc);  // open keyboard and time interrupt
	//outb(PIC_S_DATA, 0xff);

	put_str("pic_init end\n");
}

enum intr_status intr_enable()
{
	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;
		return old_status;
	}else{
		old_status = INTR_OFF;
		asm volatile("sti");
		return old_status;
	}
}

enum intr_status intr_disable()
{
	enum intr_status old_status;
	if(INTR_ON == intr_get_status()){
		old_status = INTR_ON;
		asm volatile("cli":::"memory");
		return old_status;
	}else{
		old_status = INTR_OFF;
		return old_status;
	}
}
enum intr_status intr_set_status(enum intr_status status){
	return status & INTR_ON ? intr_enable() : intr_disable();
}

enum intr_status intr_get_status(){
	uint32_t eflags = 0;
	GET_EFLAGS(eflags);
	return (EFLAGS_IF & eflags) ? INTR_ON : INTR_OFF;
}

void idt_init()
{
	put_str("start idt\n");
	idt_desc_init();
	exception_init();
	pic_init();
	
	uint64_t idt_operand = ((sizeof(idt) -1) | ((uint64_t)((uint32_t)idt) << 16));
	asm volatile("lidt %0" : : "m"(idt_operand));
	put_str("end idt\n");
}


