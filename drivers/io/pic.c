#include <drivers/io/pic.h>
#include <utils/ports.h>
#include <drivers/video.h>

void remapPIC()
{
    // 0x20 : Master PIC Command Port
    // 0x21 : Master PIC Data Port
    // 0xA0 : Slave PIC Command Port
    // 0xA1 : Slave PIC Data Port
    
    uint8_t m1, m2; // Save the original masks
    m1 = inb(0x21); 
    m2 = inb(0xA1);
    // Start initialisation by sending ICW1 (IC4 | 0x10)
    outb(0x20, 0x11); 
    ioWait();
    outb(0xA0, 0x11);
    ioWait();

    // Send ICW2
    outb(0x21, 0x20); // Set master PIC interrupt offset to 0x20
    ioWait();
    outb(0xA1, 0x28); // Set master PIC interrupt offset to 0x28
    ioWait();
    
    // Send ICW3 (TODO: Set up cascading)
    outb(0x21, 0x00); 
    ioWait();
    outb(0xA1, 0x00);
    ioWait();
    
    // Send ICW4 (Set 8086 Mode)
    outb(0x21, 0x01);
    ioWait();
    outb(0xA1, 0x01);
    ioWait();

    outb(0x21, m1); // Restore masks
    ioWait();
    outb(0xA1, m2);
    ioWait();
}
