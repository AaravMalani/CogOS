#ifndef __IO
#define __IO
#include "../utils/types.h"

void remapPIC(); // Re-map the master and slave PICs to handle interrupts 0x20-0x27 and 0x28-0x2F respectively
void loadIDT(); // Generate the IDT and load the IDTR with the IDT 
#endif