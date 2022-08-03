;kernel.asm
;Michael Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

	.global _putInMemory

;void putInMemory (int segment, int address, char character)
; ax = segment
; si = address;
; cl = character

_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds

	; GWB: added this to allow function calls and 
	; parameter passing in the first project.
	pop bp

	ret
