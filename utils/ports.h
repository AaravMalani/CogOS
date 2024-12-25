#ifndef __PORTS
#define __PORTS

#include "types.h"

uint8_t inb(uint16_t address);
uint16_t inw(uint16_t address);
uint32_t inl(uint16_t address);
void outb(uint16_t address, uint8_t data);
void outw(uint16_t address, uint16_t data);
void outl(uint16_t address, uint32_t data);
void ioWait();

#endif