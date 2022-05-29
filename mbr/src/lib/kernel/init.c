#include "init.h"
#include "print.h"
#include "interrupt.h"
#include "thread.h"
#include "memory.h"
#include "fs.h"
#include "../../device/keyboard.h"
#include "console.h"
#include "../../device/timer.h"
#include "../user/tss.h"
#include "../user/syscall-init.h"
#include "../../device/ide.h"
void init_all(void){
	put_str("init all!\n");
	idt_init();
	timer_init();
	mem_init();
	thread_init();
	console_init();
	keyboard_init();
	tss_init();
	syscall_init();
	ide_init();
}
