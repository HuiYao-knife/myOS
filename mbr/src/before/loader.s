%include "boot.inc"
section loader vstart=LOADER_BASE_ADDR
LOADER_STACK_TOP equ LOADER_BASE_ADDR
jmp	loader_start

GDT_BASE: dd	0x00000000
	  dd	0x00000000
CODE_DESC:dd	0x0000FFFF
	  dd	DESC_CODE_HIGH4
DATA_STACK_DESC:dd	0x0000FFFF
	  dd	DESC_DATA_HIGH4
VIDEO_DESC: dd	0x80000007
	  dd	DESC_VIDEO_HIGH4
GDT_SIZE	equ	$ - GDT_BASE
GDT_LIMIT	equ	GDT_SIZE-1

times 512-($-$$) db 0

total_mem_bytes dd 0
gdt_ptr   dw	GDT_LIMIT
	  dd	GDT_BASE
ards_buf  times 244 db 0
ards_nr   dw	0



SELECTOR_CODE	equ	(0x0001<<3) + TI_GDT + RPL0
SELECTOR_DATA	equ	(0x0002<<3) + TI_GDT + RPL0
SELECTOR_VIDEO	equ	(0x0003<<3) + TI_GDT + RPL0


loadermsg dd	'2 loader in real.'

loader_start:
	xor	ebx,ebx
	mov	di,ards_buf
	mov	edx,0x534d4150
mem_check_e820:
	mov	eax,0xe820
	mov	ecx,20
	int	0x15
	jc	mem_cal_e820
	inc	word [ards_nr]
	add	di,cx
	
	cmp	ebx,0
	jnz	mem_check_e820

	mov	ebx,ards_buf
	mov	cx,[ards_nr]
	xor	edx,edx
mem_cal_e820:
	mov	eax,[ebx]
	add	eax,[ebx + 8]
	add	ebx,20
	cmp	edx,eax
	jae	mem_loop_e820
	mov	edx,eax
mem_loop_e820:	
	loop	mem_cal_e820
	jmp	mem_get_ok

mem_check_e801:
	mov	eax,0xe801
	int	0x15
	jc	mem_check_88

	mov	cx,0x400
	mul	cx
	shl	edx,16
	or	edx,eax
	add	edx,0x100000
	mov	esi,edx
	
	mov	eax,0x10000
	mul	ebx
	mov	edx,esi
	add	edx,eax
	jmp	mem_get_ok

mem_check_88:
	mov	ah,0x88
	int	0x15
	jc	error_hlt
	mov	cx,0x400
	mul	cx
	shl	edx,16
	or	edx,eax
	add	edx,0x100000
	jmp	mem_get_ok

mem_get_ok:
	mov	[total_mem_bytes],edx
	

	mov	ax,0x1301
	mov	bx,0x001f
	mov	cx,17
	mov	dx,0x1800
	mov	bp,loadermsg
	int 	0x10
	
	in	al,0x92
	or	al,0000_0010b
	out	0x92,al

	lgdt	[gdt_ptr]

	mov	eax,cr0
	or	eax,0x00000001
	mov	cr0,eax

	jmp dword SELECTOR_CODE:p_mode_start

[bits 32]
p_mode_start:
	mov	ax,SELECTOR_DATA
	mov	ds,ax
	mov 	ss,ax
	mov	es,ax
	mov	esp,LOADER_STACK_TOP
	mov	ax,SELECTOR_VIDEO
	mov 	gs,ax
load_kernel:
	mov eax,KERNEL_START_SECTOR		;0x9
	mov ebx,KERNEL_BIN_BASE_ADDR	;0x70000
	mov ecx,200
	call rd_disk_m_32
page_mode_start:
	call	setup_page
	sgdt	[gdt_ptr]
	mov	ebx,[gdt_ptr + 2]
	or	dword [ebx + 0x18 + 4],0xc0000000	
	add	dword [gdt_ptr + 2],0xc0000000
	add	esp,0xc0000000
	mov	eax,PAGE_DIR_TABLE_POS
	mov	cr3,eax
	mov	eax,cr0
	or	eax,0x80000000
	mov	cr0,eax
	lgdt	[gdt_ptr]
	jmp	SELECTOR_CODE:enter_kernel
enter_kernel:
	call	kernel_init
	mov	esp,0xc009f000
	jmp	KERNEL_ENTRY_POINT       ;0x1500
	mov	byte [gs:160],'x'
	mov	byte [gs:162],'c'
	mov	byte [gs:164],'r'
error_hlt:
	jmp	$
	



setup_page:				;setup_page
	mov	ebx,PAGE_DIR_TABLE_POS
	mov	ecx,4096
	mov	esi,0
set_dir_zero:	
	mov	byte [ebx + esi],0
	inc	esi
	loop	set_dir_zero
create_pde:
	mov	eax,ebx
	add	eax,0x1000
	or	eax,PG_US_U | PG_RW_W | PG_P
	mov	[ebx],eax
	mov	[ebx + 0xc00],eax
	sub	eax,0x1000
	mov	[ebx + 4092],eax
create_pte:
	mov	ecx,256
	mov	esi,0
	mov	edx,PG_US_U | PG_RW_W | PG_P
create_pte_loop:
	mov	[ebx + 0x1000 + esi * 4],edx
	add	edx,0x1000
	inc	esi
	loop	create_pte_loop
create_c00_pde:
	mov	eax,ebx
	add	eax,0x2000
	or	eax,PG_US_U | PG_RW_W | PG_P
	mov	ecx,254
	mov	esi,769
c00_pde_loop:
	mov	[ebx + esi * 4],eax
	add	eax,0x1000
	inc	esi
	loop	c00_pde_loop 
	ret

kernel_init:			;init_kernel
	xor	eax,eax
	xor	ebx,ebx
	xor	ecx,ecx
	xor	edx,edx
	mov	ebx,[KERNEL_BIN_BASE_ADDR + 28]
	add	ebx,KERNEL_BIN_BASE_ADDR
	mov	cx,[KERNEL_BIN_BASE_ADDR + 44]
	mov	dx,[KERNEL_BIN_BASE_ADDR + 42]
load_kernel_loop:
	cmp	byte [ebx],PT_NULL
	je	loop_inc
	
	push	dword [ebx + 16]
	mov	eax,[ebx + 4]
	add	eax,KERNEL_BIN_BASE_ADDR
	push	eax
	push	dword [ebx + 8]
	call	mem_cpy
	add	esp,12
loop_inc:
	add	ebx,edx
	loop	load_kernel_loop
	ret


mem_cpy:		;mem_cpy
	cld
	push	ebp
	mov	ebp,esp
	push	ecx
	mov	edi,[ebp + 8]
	mov	esi,[ebp + 12]
	mov	ecx,[ebp + 16]
	rep	movsb
	pop	ecx
	pop	ebp
	ret

rd_disk_m_32:	     ;rd_disk_m_32
	mov esi,eax
	mov di,cx
	
	mov dx,0x1f2
	mov al,cl
	out dx,al

	mov eax,esi

	mov dx,0x1f3
	out dx,al

	mov cl,8
	shr eax,cl
	mov dx,0x1f4
	out dx,al

	shr eax,cl
	mov dx,0x1f5
	out dx,al

	mov dx,0x1f6
	shr eax,cl
	and al,0x0f
	or al,0xe0
	out dx,al

	mov dx,0x1f7
	mov al,0x20
	out dx,al

not_ready:
	nop
	in al,dx
	and al,0x88
	cmp al,0x08
	jne not_ready

	mov ax,di
	mov dx,256
	mul dx
	mov cx,ax
	mov dx,0x1f0

go_on_read:
	in ax,dx
	mov [ebx],ax
	add ebx,2
	loop go_on_read
	ret

