[bits 16]
[org 0x7c00]

xor ax, ax
mov ds, ax
mov es, ax
mov ss, ax

;Read 1 sector from hard drive
mov ax, 0x0210
mov cx, 2
mov dh, 0
xor bx, bx
mov es, bx
mov bx, 0x7e00
int 0x13
jc harddrive_err

cli
lgdt [gdtr]
mov ebx, cr0
or ebx, 1
mov cr0, ebx

mov ax, 0x10 
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
jmp 0x8:0x7e00

harddrive_err:
mov si, HARDDRIVE_1
call printf
jmp $

printf:
pusha
xor bx, bx
mov ah, 0x0e

printf_loop:
lodsb
test al, al
jz printf_end
int 0x10
jmp printf_loop


printf_end:
popa
ret

gdt_start:

dq 0

dw 0xffff
dw 0
db 0
db 0x9a
db 0xcf
db 0

dw 0xffff
dw 0
db 0
db 0x92
db 0xcf
db 0
gdtr:
dw gdtr-gdt_start-1
dd gdt_start

HARDDRIVE_1: db "Error reading hard drive!", 0xa, 0xd, 0
times 510-($-$$) db 0
dw 0xaa55