[bits 32]

global keyboard_handler
global error_handler
extern keyboard_handler_c
keyboard_handler:
call keyboard_handler_c
iretd

error_handler:
cli
hlt
iretd