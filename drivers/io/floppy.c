#include <drivers/video.h>
#include <utils/types.h>
#include <utils/ports.h>

uint8_t floppyFind() {
    outb(0x70, (1 << 7) | 0x10);
    ioWait();
    return inb(0x71);
}
