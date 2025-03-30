#ifndef __IO_PIC
#define __IO_PIC
#include <utils/types.h>

void remapPIC(); // Re-map the master and slave PICs to handle interrupts 0x20-0x27 and 0x28-0x2F respectively

#endif