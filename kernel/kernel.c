#include "../drivers/video.h"
#include "../drivers/io.h"
#include "../drivers/mm.h"
int main(void)
{
    initPaging(); // Initialise paging 
    clear(0x3b); // Clear the screen
    print("CogOS: A 32 Bit OS!\n"); 
    remapPIC(); // Remap the PIC
    loadIDT(); // Load Interrupt Descriptor Table
    readMemory(); // Print memory map
    while (1);
}