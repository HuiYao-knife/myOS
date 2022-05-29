TI_GDT	equ	0
RPL0	equ	0
SELECTOR_VIDEO	equ	(0x0003<<3) + TI_GDT + RPL0

[bits 32]
section .data
put_int_buffer dq 0

section .text
global put_char
put_char:
pushad
mov	ax,SELECTOR_VIDEO
mov	gs,ax

mov	dx,0x03d4
mov	al,0x0e
out	dx,al
mov	dx,0x03d5
in	al,dx
mov	ah,al
mov	dx,0x3d4
mov	al,0x0f
out	dx,al
mov	dx,0x03d5
in	al,dx
mov	bx,ax

mov	ecx,[esp + 36]
cmp	cl,0x0d
jz	carriage
cmp	cl,0x0a
jz	line_feed
cmp	cl,0x08
jz	backspace
jmp	print_char


backspace:
dec	bx
shl	bx,1
mov	byte [gs:bx],0x20
mov	byte [gs:bx + 1],0x07
shr	bx,1
jmp	set_cursor


print_char:
shl	bx,1
mov	byte [gs:bx],cl
mov	byte [gs:bx + 1],0x7
shr	bx,1
inc	bx
cmp	bx,2000
jl	set_cursor

line_feed:
carriage:
xor	edx,edx
mov	si,80
mov	ax,bx
div	si
sub	bx,dx

add	bx,80
cmp	bx,2000
jl	set_cursor

full_screen:
cld
mov	esi,0xc00b80a0
mov	edi,0xc00b8000
mov	ecx,960
rep	movsd
mov	ecx,80
mov	ebx,3840
fs_loop:
mov	word [gs:ebx],0x0720
add	ebx,2
loop	fs_loop
mov	bx,1920


set_cursor:
mov	dx,0x3d4
mov	al,0x0e
out	dx,al
mov	dx,0x3d5
mov	al,bh
out	dx,al

mov	dx,0x3d4
mov	al,0x0f
out	dx,al
mov	dx,0x3d5
mov	al,bl
out	dx,al
popad
ret

global put_str
put_str:
push	ebx
push	ecx
xor	ecx,ecx
mov	ebx,[esp + 12]
str_loop:
mov	cl,[ebx]
cmp	cl,0
jz	str_end
push	ecx
call	put_char
add	esp,4
inc	ebx
jmp	str_loop
str_end:
pop	ecx
pop	ebx
ret

global put_int
put_int:
pushad
mov	ebp,esp
mov	eax,[ebp + 36]
mov	edx,eax
mov	edi,7
mov	ecx,8
mov	ebx,put_int_buffer
in_buffer_loop:
and	edx,0x0000000f
cmp	edx,9
jg	ascii_A2F
add	edx,'0'
jmp	store
ascii_A2F:
sub	edx,10
add	edx,'a'
store:
mov	[ebx + edi],dl
dec	edi
shr	eax,4
mov	edx,eax
loop	in_buffer_loop
print_from_buf:
inc	edi
skip_prefix:
cmp	edi,8
je	full0
go_on_skip:
mov	cl,[put_int_buffer + edi]
inc	edi
cmp	cl,'0'
je	skip_prefix
dec	edi
jmp	put_char_loop
full0:
mov	cl,'0'
put_char_loop:
push	ecx
call	put_char
add	esp,4
inc	edi
mov	cl,[put_int_buffer + edi]
cmp	edi,8
jl 	put_char_loop
popad
ret

global gset_cursor
gset_cursor:
mov	ebx,[esp + 4]
mov	dx,0x3d4
mov	al,0x0e
out	dx,al
mov	dx,0x3d5
mov	al,bh
out	dx,al

mov	dx,0x3d4
mov	al,0x0f
out	dx,al
mov	dx,0x3d5
mov	al,bl
out	dx,al
ret

global cls_screen
cls_screen;
pushad
mov	ax,SELECTOR_VIDEO
mov	gs,ax
mov	ebx,0
mov	ecx,80 * 25
.cls:
mov	word	[gs:ebx], 0x0720
add	ebx,2
loop	.cls
mov	ebx,0
.set_cursor:
mov	dx,0x3d4
mov	al,0xe
out	dx,al
mov	dx,0x3d5
mov	al,bh
out	dx,al

mov	dx,0x3d4
mov	al,0xf
out	dx,al
mov	dx,0x3d5
mov	al,bl
out	dx,al
popad
ret
