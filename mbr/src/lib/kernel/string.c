#include "string.h"
#include "stdint.h"
#include "global.h"
#include "debug.h"
void memset(void * dst_, uint8_t value, uint32_t size)
{
	ASSERT(dst_ != NULL);
	uint8_t * dst = (uint8_t *)dst_;
	while(size--){
		*dst = value;
		dst++;
	}
}

void memcpy(void * dst_, const void * src_, uint32_t size)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	uint8_t *dst = dst_;
	const uint8_t *src = src_;
	while(size--){
		*dst = *src;
		dst++;
		src++;
	}
}

int memcmp(const void* a_, const void* b_, uint32_t size)
{
	ASSERT(a_ != NULL && b_ != NULL);
	const uint8_t *a = a_;
	const uint8_t *b = b_;
	while(size--){
		if(*a != *b){
			return *a > *b ? 1 : -1;
		}
		a++;
		b++;
	}
	return 0;
}
		

char * strcpy(char * dst_, const char* src_)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	char * dst = dst_;
	while((*dst_++ = *src_++));
	return dst;
}

uint32_t strlen(const char * str)
{
	ASSERT(str != NULL);
	uint32_t num = 0;
	while(*(str + num)){
		num++;
	}
	return num;
}
	

int8_t strcmp(const char* a, const char * b)
{
	ASSERT(a != NULL && b != NULL);
	while(*a  && *b && *a == *b){
		a++;
		b++;
	}
	return *a < *b ? -1 : *a > *b;
}

char * strchr(const char * str, const uint8_t ch)
{
	ASSERT(str != NULL);
	while(*str){
		if(*str == ch){
			return (char * )str;
		}
		str++;
	}
	return NULL;
}

char * strrchr(const char * str, const uint8_t ch)
{
	ASSERT(str != NULL);
	const char* last_char = NULL;
	while(*str){
		if(*str == ch){
			last_char = str;
		}
		str++;
	}
	return (char*)last_char;
}

char * strcat(char * dst_, const char * src_)
{
	ASSERT(dst_ != NULL && src_ != NULL);
	char * str = dst_;
	while(*str++);
	--str;
	while((*str++ = *src_++));
	return dst_;
}

uint32_t strchrs(const char * str, uint8_t ch)
{
	ASSERT(str != NULL);
	uint32_t num = 0;
	const char * p = str;
	while(*p){
		if(*p == ch){
			num++;
		}
		p++;
	}
	return num;
}


