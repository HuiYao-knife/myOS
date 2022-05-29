#include "debug.h"
#include "interrupt.h"
#include "print.h"
#include "mystdio.h"
void panic_spin(char * filename, int line, const char * func, const char * condition)
{
	/*intr_disable();
	put_str("error!!!");
	put_str("filename: ");put_str(filename);put_str("\n");
	put_str("line: 0x");put_int(line);put_str("\n");
	put_str("function: ");put_str((char * )func);put_str("\n");
	put_str("condition: ");put_str((char * )condition);put_str("\n");*/
	printf("error!!!\nfilename:%s\nline:0x%x\nfunction:%s\ncondition:%s\n", filename, line, (char*)func, (char*)condition);
	while(1);
}
