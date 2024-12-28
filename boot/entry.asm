[bits 32]
extern main 

call main ; Enter the kernel
jmp $ ; If the kernel exits, halt indefinitely


