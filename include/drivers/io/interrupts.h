#ifndef __IO_INTERRUPTS
#define __IO_INTERRUPTS
#include <utils/types.h>

void loadIDT(); // Generate the IDT and load the IDTR with the IDT 
#endif