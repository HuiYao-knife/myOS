#ifndef	__CONSOLE_H
#define	__CONSOLE_H
#include "print.h"
void console_put_int(uint32_t num);
void console_put_str(char * s);
void console_put_char(uint8_t ch);
void console_release();
void console_init();
void console_acquire();

#endif
