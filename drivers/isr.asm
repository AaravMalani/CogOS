[bits 32]

global keyboard_handler
global error_handler
extern keyboard_handler_c

; Handles keyboard at IRQ1 (interrupt 0x21)
keyboard_handler:
call keyboard_handler_c ; Call the C handler
iretd ; Interrupt return

; Handles exceptions (interrupts 0x00 to 0x1F)
error_handler:
cli ; Disable interrupts
hlt ; Halts CPU indefinitely
iretd ; Interrupt return