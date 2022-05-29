#ifndef __KEYBOARD_H
#define	__KEYBOARD_H
#include "stdint.h"
#include "ioqueue.h"
#define	esc		'\033'
#define	tab		'\t'
#define	backspace	'\b'
#define	enter		'\r'
#define	delete		'\0177'

#define	char_invisible	0
#define	shift_l_char	0
#define	shift_r_char	0
#define	ctrl_l_char	0
#define	ctrl_r_char	0
#define	alt_l_char	0
#define	alt_r_char	0
#define	caps_char	0

#define	shift_l_make	0x2a
#define	shift_r_make	0x36
#define	shift_l_break	0xaa
#define	shift_r_break	0xb6
#define	ctrl_l_make	0x1d
#define	ctrl_r_make	0xe01d
#define	ctrl_l_break	0x9d
#define	ctrl_r_break	0xe09d
#define	alt_l_make	0x38
#define	alt_r_make	0xe038
#define	alt_l_break	0xb8
#define	alt_r_break	0xe0b8
#define	caps_make	0x3a	

static  bool ctrl_status, shift_status, alt_status, caps_status, ext_scancode;
static void intr_keyboard_handler(void);
void keyboard_init();
extern struct ioqueue kbd_buf;
#endif	
