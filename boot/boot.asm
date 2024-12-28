[bits 16]
[org 0x7c00]

xor ax, ax ; Reset segment registers
mov ds, ax
mov es, ax
mov ss, ax

; Load sectors from hard drive to 0x8000 (INT 13H, AH=0x02)
mov ax, 0x0210 ; AL = 0x10 (Read 16 sectors)
mov cx, 2 ; CH = 0 (Cylinder 0), CL = 2 (Sector 2 onwards, Sector 1 is boot sector)
mov dh, 0 ; DH = 0 (Head 0)
xor bx, bx 
mov es, bx ; Set ES:BX TO 0x0:0x8000
mov bx, 0x8000 
int 0x13 ; Call INT 13H
jc harddrive_err ; In case of error, carry flag is set

; Read memory map to 0x20000 (INT 15H, EAX=0xE820)
mov bx, 0x2000 
mov es, bx ; Set ES:DI to 0x2000:0x0  
xor di, di
xor ebx, ebx ; Set EBX to 0
mov edx, 0x534D4150 ; Set EDX to "SMAP" 
mov eax, 0xE820 ; Set EAX to 0xE820
mov ecx, 24 ; Read 24 bytes (include ACPI 3.0 Extended Attributes)
int 0x15 ; Call interrupt 
jc ebfe ; TODO: Write better exception handler | If carry flag is set, the interrupt is not successful
cmp eax, 0x534D4150 ; If EAX is not set to "SMAP", the interrupt is not successful
jne ebfe  

mem_detect_loop:
movzx cx, cl ; CL contains the entry size
add di, cx ; Add CL to the pointer
mov eax, 0xe820 ; Reset EAX to E820 
mov ecx, 24 ; Reset ECX to 24
int 0x15 ; Call interrupt 
jc mem_detect_mid ; If carry flag is set, we have gone past the last entry
test ebx, ebx ; If EBX is 0, we have just stored the last entry
jnz mem_detect_loop ; Read the next entry
jmp mem_detect_end 
mem_detect_mid: 
; If carry flag was set, we incremented DI by CL one time extra
movzx cx, cl 
sub di, cx ; Subtract DI by CL
mem_detect_end:
movzx cx, cl 
mov bx, 0x1999 
mov es, bx
mov [es:8], di ; Store (index of the last element)*size to 0x19998 
mov [es:6], cx ; Store the size of the entries to 0x19996
cli ; Disable interrupts
lgdt [gdtr] ; Load GDTR 
mov ebx, cr0 
or ebx, 1 ; Set the PE (Protected Mode Enable) flag in CR0
mov cr0, ebx 

mov ax, 0x10 ; Set our data segment registers to the data segment (0x10) 
mov ds, ax 
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax
jmp 0x8:0x8000 ; Long jump to our newly loaded sectors to set the CS to 0x08

harddrive_err:
mov si, HARDDRIVE_1 
call printf 
ebfe: 
jmp $



printf: ; Prints null-terminated string stored at address in SI
pusha ; Store all registers on the stack
mov ah, 0x0e ; Set AH = 0x0E (INT 10H, AH = 0x0E)

printf_loop: 
lodsb ; Loads a character from address at SI and increments SI
test al, al ; If AL is 0, go to printf_end
jz printf_end
int 0x10 ; Call interrupt to print character in AL
jmp printf_loop ; Go to the start of the loop 


printf_end:
popa ; Loads the registers from the stack
ret ; Returns to calling instruction

HARDDRIVE_1: db "Error reading hard drive!", 0xa, 0xd, 0

times (510-gdt_end+gdt_start) - ($ - $$) db 0 ; Move the GDT to the end of the file 

gdt_start:

; Null Segment (Entry 0, Offset 0x00)
dq 0 

; Kernel Code Segment (Entry 1, Offset 0x08)
dw 0xffff ; Low 16 bits of Limit
dw 0x0000 ; Low 24 bits of Base
db 0x00   
db 0x9a   ; Access Byte (RW, E, S, P)
db 0xcf   ; High 4 bits of Limit and Flags (DB, G)
db 0x00   ; High 8 bits of Base

; Kernel Data Segment (Entry 2, Offset 0x10)
dw 0xffff ; Low 16 bits of Limit
dw 0x0000 ; Low 24 bits of Base
db 0x00
db 0x92   ; Access Byte (RW, S, P)
db 0xcf   ; High 4 bits of Limit and Flags (DB, G)
db 0x00   ; High 8 bits of Base

; GDT Register
gdtr:
dw gdtr-gdt_start-1 ; Stores one less than the GDT's size 
dd gdt_start        ; Stores the base of the GDT
gdt_end:

times 510-($-$$) db 0 ; Set the rest of the bootsector to 0
dw 0xaa55 ; Set the last two bytes to the signature