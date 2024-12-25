#include "io.h"
#include "../utils/ports.h"
#include "video.h"
struct IDTDescriptor
{
    uint16_t size;
    uint32_t offset;
} __attribute__((packed));

struct IDTEntry
{
    uint16_t offset1;
    uint16_t segmentSelector;
    uint8_t reserved;
    uint8_t flags;
    uint16_t offset2;
} __attribute__((packed));

__attribute__((aligned(0x10))) static struct IDTEntry idt[256];
static struct IDTDescriptor idtr;

__attribute__((noreturn)) extern void error_handler(void);
extern void keyboard_handler(void);
// From http://www.osdever.net/bkerndev/Docs/keyboard.htm
unsigned char kbdus[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

uint8_t shift, capslock;
void keyboard_handler_c(void)
{
    uint8_t ch = inb(0x60);
    outb(0x20, 0x20);
    if (ch == 42)
    {
        shift = true;
    }
    else if (ch == (42 | 0x80))
    {
        shift = false;

    }

    if (ch == 58) {
        capslock = !capslock;
    }
    if (ch & 0x80)
    {
        return;
    }
    if (kbdus[ch])
    {
        char s[2] = {kbdus[ch], 0};
        if (kbdus[ch] >= 'a' && kbdus[ch] <= 'z' && (shift ^ capslock))
        {
            s[0] -= 32;
        }
        print(s);
    }
}
void loadIDT()
{
    for (int i = 0; i < 32; i++)
    {
        idt[i].offset1 = (uint32_t)error_handler & 0xFFFF;
        idt[i].segmentSelector = 0x08;
        idt[i].reserved = 0;
        idt[i].flags = 0x8E;
        idt[i].offset2 = (uint32_t)error_handler >> 16;
    }
    idt[0x21].offset1 = (uint32_t)keyboard_handler & 0xFFFF;
    idt[0x21].segmentSelector = 0x08;
    idt[0x21].reserved = 0;
    idt[0x21].flags = 0x8E;
    idt[0x21].offset2 = (uint32_t)keyboard_handler >> 16;
    idtr.size = sizeof(idt) - 1;
    idtr.offset = (uint32_t)&idt[0];
    shift = 0, capslock = 0;
    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");

    outb(0x21, 0xfd);
}

void remapPIC()
{
    uint8_t m1, m2;
    m1 = inb(0x21);
    m2 = inb(0xA1);
    outb(0x20, 0x11);
    ioWait();
    outb(0xA0, 0x11);
    ioWait();
    outb(0x21, 0x20);
    ioWait();
    outb(0xA1, 0x28);
    ioWait();
    outb(0x21, 0x00);
    ioWait();
    outb(0xA1, 0x00);
    ioWait();
    outb(0x21, 0x01);
    ioWait();
    outb(0xA1, 0x01);
    ioWait();

    outb(0x21, m1);
    ioWait();
    outb(0xA1, m2);
    ioWait();
}
