#include "kstdio.h"
#include "mystdio.h"
#include "console.h"
#include "stdint.h"
#include "global.h"
void printk(const char * format, ...){
	char *  args;
	va_start(args, format);
	char buf[1024] = {0};
	vsprintf(buf, format, args);
	va_end(args);	
	console_put_str(buf);
}

void sys_putchar(const char chr){
	console_put_char(chr);
}
