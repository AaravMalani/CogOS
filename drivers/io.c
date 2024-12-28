#include "io.h"
#include "../utils/ports.h"
#include "video.h"

struct IDTDescriptor
{
    uint16_t size;   // One minus the size of the IDT
    uint32_t offset; // The base address of the IDT
} __attribute__((packed));

struct IDTEntry
{
    uint16_t offset1; // Low 16 bits of ISR address
    uint16_t segmentSelector;
    uint8_t reserved; // Set to 0
    uint8_t flags;    // Type attributes
    uint16_t offset2; // High 16 bits of ISR address
} __attribute__((packed));

__attribute__((aligned(0x10))) static struct IDTEntry idt[256];
static struct IDTDescriptor idtr;

__attribute__((noreturn)) extern void error_handler(void); // Handles exceptions (Interrupts 0x00-0x1F)
extern void keyboard_handler(void);                        // Handles keyboard inputs (IRQ 1, Interrupt 0x21)
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

uint8_t shift, capslock; // States of shift key and caps-lock
void keyboard_handler_c(void)
{
    uint8_t ch = inb(0x60); // Read the data from the keyboard data port
    outb(0x20, 0x20);       // Send the EOI signal to master PIC
    if (ch == 42)           // Shift pressed
    {
        shift = true;
    }
    else if (ch == (42 | 0x80)) // Shift released
    {
        shift = false;
    }

    if (ch == 58)
    { // Caps lock pressed
        capslock = !capslock;
    }
    if (ch & 0x80) // For now, we discard key releases
    {
        return;
    }
    if (kbdus[ch]) // If there is an ASCII representation for it, print it
    {
        char s[2] = {kbdus[ch], 0}; // 2-byte string with character
        if (kbdus[ch] >= 'a' && kbdus[ch] <= 'z' && (shift ^ capslock))
        {
            // Either shift or caps-lock was active
            s[0] -= 32; // Convert to uppercase
        }
        // TODO: Implement shift for other characters as well
        print(s);
    }
}
void loadIDT()
{
    for (int i = 0; i < 32; i++) // Load IDT for interrupts 0x00 - 0x1F (Intel reserved exceptions)
    {
        idt[i].offset1 = (uint32_t)error_handler & 0xFFFF; // Load low 16 bits of exception handler
        idt[i].segmentSelector = 0x08;                     // Point to the code segment in the GDT
        idt[i].reserved = 0;                               // Set the reserved bytes to 0
        idt[i].flags = 0x8E;                               // Set type attributes (Gate Type = 0xE, P)
        idt[i].offset2 = (uint32_t)error_handler >> 16;    // Load high 16 bits of exception handler
    }

    // Load IDT with keyboard handler for interrupt 0x21 (IRQ 1)
    idt[0x21].offset1 = (uint32_t)keyboard_handler & 0xFFFF; // Load low 16 bits of keyboard handler
    idt[0x21].segmentSelector = 0x08;                        // Point to the code segment in the GDT
    idt[0x21].reserved = 0;                                  // Set the reserved bytes to 0
    idt[0x21].flags = 0x8E;                                  // Set type attributes (Gate Type = 0xE, P)
    idt[0x21].offset2 = (uint32_t)keyboard_handler >> 16;    // Load high 16 bits of keyboard handler
    
    idtr.size = sizeof(idt) - 1; // Load IDT's size
    idtr.offset = idt;           // Load base address of IDT

    __asm__ volatile("lidt %0" : : "m"(idtr)); // Load the IDTR
    __asm__ volatile("sti"); // Enable interrupts

    shift = 0, capslock = 0;     // Reset the shift and caps-lock state
    outb(0x21, 0xfd); // Unmask IRQ 1 (PS/2 Keyboard)
}

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
