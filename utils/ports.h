#ifndef __PORTS
#define __PORTS

#include "types.h"

uint8_t inb(uint16_t address); // Accepts a 8-bit value from a port
uint16_t inw(uint16_t address); // Accepts a 16-bit value from a port
uint32_t inl(uint16_t address); // Accepts a 32-bit value from a port
void outb(uint16_t address, uint8_t data); // Passes a 8-bit value to a port
void outw(uint16_t address, uint16_t data); // Passes a 16-bit value to a port 
void outl(uint16_t address, uint32_t data); // Passes a 32-bit value to a port
void ioWait(); // Gives time for the PIC to react in the case of older machines

#endif