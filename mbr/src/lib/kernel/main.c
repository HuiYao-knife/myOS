#include "mystdio.h"
#include "init.h"
#include "debug.h"
#include "memory.h"
#include "interrupt.h"
#include "thread.h"
#include "keyboard.h"
#include "process.h"
#include "console.h"
#include "buildin_cmd.h"
#include "ioqueue.h"
#include "syscall-init.h"
#include "file.h"
#include "fs.h"
#include "fork.h"
#include "shell.h"
#include "mystdio.h"
#include "syscall.h"
#include "kstdio.h"

int main(void){
put_str("I am kernel\n");
init_all();

intr_enable();
/*int fd1 = sys_open("/hello", O_CREAT|O_RDWR);
sys_write(fd1, "hello,world", 11);
sys_close(fd1);*/
uint32_t file_size = 8440;
uint32_t sec_cnt = DIV_ROUND_UP(file_size, 512);
struct disk * sda = &channels[0].devices[0];
void * prog_buf = sys_malloc(sec_cnt * 512);
//printf("\n%x  %d   %d\n", prog_buf, sec_cnt, file_size);
ide_read(sda, 300, prog_buf, sec_cnt);
int32_t fd = sys_open("/cat", O_CREAT|O_RDWR);
if(fd != -1){
	if(sys_write(fd, prog_buf, file_size) == -1){
		printk("file write error!\n");
		while(1);
	}
}
cls_screen();

console_put_str("[os@localhost /]$ ");

while(1);
return 0;
}


