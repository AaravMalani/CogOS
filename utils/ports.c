#include "ports.h"

uint8_t inb(uint16_t address) {
    uint8_t val;
    __asm__ volatile("inb %1, %0" : "=a" (val): "d" (address));
    return val;
}
uint16_t inw(uint16_t address) {
    uint16_t val;
    __asm__ volatile("inw %1, %0" : "=a" (val): "d" (address));
    return val;
}
uint32_t inl(uint16_t address) {
    uint32_t val;
    __asm__ volatile("inl %1, %0" : "=a" (val): "d" (address));
    return val;
}
void outb(uint16_t address, uint8_t data) {
    __asm__ volatile("outb %0, %1" :: "a" (data), "d" (address));
}
void outw(uint16_t address, uint16_t data) {
    __asm__ volatile("outw %0, %1" :: "a" (data), "d" (address));
}
void outl(uint16_t address, uint32_t data) {
    __asm__ volatile("outl %0, %1" :: "a" (data),"d" (address));
}

void ioWait() {
    outb(0x80, 0xFF);
}