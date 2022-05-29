#include "keyboard.h"
#include "../lib/kernel/ioqueue.h"
#include "../lib/kernel/print.h"
#include "../lib/kernel/interrupt.h"
#include "../lib/kernel/io.h"
#include "../lib/kernel/global.h"

#define	KBD_BUF_PORT	0x60
static char keymap[][2] = {
{0,	0},
{esc,	esc},
{'1',	'!'},
{'2',	'@'},
{'3',	'#'},
{'4',	'$'},
{'5',	'%'},
{'6',	'^'},
{'7',	'&'},
{'8',	'*'},
{'9',	'('},
{'0',	')'},
{'-',	'_'},
{'=',	'+'},
{backspace, backspace},
{tab,	tab},
{'q',	'Q'},
{'w',	'W'},
{'e',	'E'},
{'r',	'R'},
{'t',	'T'},
{'y',	'Y'},
{'u',	'U'},
{'i',	'I'},
{'o',	'O'},
{'p',	'P'},
{'[',	'{'},
{']',	'}'},
{enter,	enter},
{ctrl_l_char, ctrl_l_char},
{'a',	'A'},
{'s',	'S'},
{'d',	'D'},
{'f',	'F'},
{'g',	'G'},
{'h',	'H'},
{'j',	'J'},
{'k',	'K'},
{'l',	'L'},
{';',	':'},
{'\'',	'"'},
{'`',	'~'},
{shift_l_char,	shift_r_char},
{'\\',	'|'},
{'z',	'Z'},
{'x',	'X'},
{'c',	'C'},
{'v',	'V'},
{'b',	'B'},
{'n',	'N'},
{'m',	'M'},
{',',	'<'},
{'.',	'>'},
{'/',	'?'},
{shift_r_char,	shift_r_char},
{'*',	'*'},
{alt_l_char,	alt_l_char},
{' ',	' '},
{caps_char,	caps_char}};


struct ioqueue kbd_buf;
static void intr_keyboard_handler(void){
	bool ctrl_down_last = ctrl_status;
	bool alt_down_last = alt_status;
	bool shift_down_last = shift_status;	
	bool caps_down_last = caps_status;

	bool breakcode;
	uint16_t scancode = inb(KBD_BUF_PORT);

	if(scancode == 0xe0){
		ext_scancode = 1;
		return;
	}

	if(ext_scancode){
		scancode = (scancode | (0xe000));
		ext_scancode = 0;
	}

	breakcode = ((scancode & 0x0080) != 0);
	if(breakcode){
		uint16_t make_code = scancode & 0xff7f;
		if(make_code == ctrl_l_make || make_code == ctrl_r_make){
			ctrl_status = 0;
		}
		if(make_code == alt_l_make || make_code == alt_r_make){
			alt_status = 0;
		}
		if(make_code == shift_l_make || make_code == shift_r_make){
			shift_status = 0;
		}
		return;
	}else if((scancode > 0x00 && scancode < 0x3b) || scancode == alt_r_make || scancode == ctrl_r_make){
		bool shift = 0;
		if(scancode < 0x0e || scancode == 0x29 || scancode == 0x1a || scancode == 0x1b || scancode == 0x2b || scancode == 0x27 || scancode == 0x28 || scancode == 0x33 || scancode == 0x34 || scancode == 0x35){
			if(shift_down_last){
				shift = 1;
			}
		}else{
			if(shift_down_last && caps_down_last){
				shift = 0;
			}else if(shift_down_last || caps_down_last){
				shift = 1;
			}else{
				shift = 0;
			}
		}
		
		uint8_t index = scancode & 0x00ff;
		char cur_char = keymap[index][shift];
		
		if(cur_char){
			if((ctrl_down_last && cur_char == 'l') || (ctrl_down_last && cur_char == 'u')){
				cur_char -= 'a';
			}
			if(!ioq_full(&kbd_buf)){
				ioq_putchar(&kbd_buf,cur_char);
			}
			return;
		}

		if(scancode == ctrl_l_make || scancode == ctrl_r_make){
			ctrl_status = 1;
		}else if(scancode == alt_l_make || scancode == alt_r_make){
			alt_status = 1;
		}else if(scancode == shift_l_make || scancode == shift_r_make){
			shift_status = 1;
		}else if(scancode == caps_make){
			caps_status = !caps_status;
		}

	}else{
		put_str("unknown key\n");
	}
}
void keyboard_init(){
	put_str("kerboard init start\n");
	ioqueue_init(&kbd_buf);
	register_handler(0x21, intr_keyboard_handler);
	put_str("keyboard init done\n");
}
