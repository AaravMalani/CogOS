[bits 32]

section .text
global enablePaging

enablePaging:
push ebp ; Set up stack frame
mov ebp, esp  
mov eax, dword [ebp+8] ; Receive the page directory from stack
mov cr3, eax ; Set CR3 to the page directory address
mov eax, cr0 
or eax, 0x80000001 ; Set PG and PE flags in CR0
mov cr0, eax 
lea eax, [newInput] ; Far jump to new virtual address
jmp eax
newInput:
add dword [ebp+4], 0xc0000000 ; Add 0xC0000000 to return value to jump back to calling function's virtual not physical address
mov eax, dword [ebp+8] ; Load page directory into EAX
mov dword [eax], 2 ; Remove the identity paging

pop ebp 
ret