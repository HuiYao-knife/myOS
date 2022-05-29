#ifndef	__MYSTDIO_H
#define	__MYSTDIO_H
#include "stdint.h"
#include "global.h"
#define	va_start(ap,v)	ap=(char *)&v
#define	va_arg(ap,t)	*((t*)(ap+=4))
#define	va_end(ap)	ap=NULL
void itoa(uint32_t value, char** buf_ptr_addr, uint8_t base);
uint32_t printf(const char * format, ...);
uint32_t vsprintf(char * str, const char * format, char * ap);
uint32_t sprintf(char * buf, const char * format, ...);
#endif
