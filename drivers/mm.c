#include "../utils/types.h"
#include "video.h"

uint32_t pageDir[1024] __attribute__((aligned(0x1000))); // The page directory
uint32_t pageTable0[1024] __attribute__((aligned(0x1000))); // Page tables 0 (for identity paging) and 768 (to map first 4MB to 0xC0000000)
struct GDTRegister { // Temporary GDTR to set address of GDT to new virtual address
    uint16_t size; // The size of the GDT - 1
    uint32_t address; // The virtual address of the GDT
} __attribute__((packed)) gdtr; 
extern void enablePaging(uint32_t pageDir); 
void initPaging()
{   
    // The weird subtraction is because the linker uses the virtual addresses
    // So until paging is set up, we need to subtract the 0xc0000000 to get the
    // physical address 
    // TODO: Find a way to get around this janky set-up  
    for (uint32_t i = 0; i < 1024; i++) // Reset page directory
    {
        ((uint32_t*)((uint32_t)pageDir - 0xc0000000))[i] = 0x00000002; // Reset the page directory entry (Flag RW) 
    }
    // Set the top 20 bits of page table along with flags RW and P
    ((uint32_t*)((uint32_t)pageDir - 0xc0000000))[0] = (((uint32_t)pageTable0 - 0xc0000000)&~(0xFFF)) | 3; // Identity page 0x00000000-0x00400000 to 0x00000000-0x00400000
    ((uint32_t*)((uint32_t)pageDir - 0xc0000000))[768] = (((uint32_t)pageTable0 - 0xc0000000)&~(0xFFF)) | 3; // Page 0x00000000-0x00400000 to 0xc0000000-0xc0400000
    for (uint32_t i = 0; i < 1024; i++) // Fill page table
    {
        ((uint32_t*)((uint32_t)pageTable0 - 0xc0000000))[i] = i * 0x1000 | 3; // Set top 20 bits of page address and flags RW and P
    }
    enablePaging((uint32_t)pageDir-0xc0000000); // Enable paging in assembly function
    gdtr.address = 0xc0007de0; // Reload the GDT with new virtual address
    gdtr.size = 0x17; 
    __asm__ volatile("lgdt %0" : : "m"(gdtr));
    __asm__ volatile("addl $0xc0000000, 0x4(%ebp)"); // Add 0xc0000000 to return value to jump to calling instruction's virtual not physical address
}

void readMemory() {
    uint16_t size = *(uint16_t*)(0xc0019996); // Read size of memory map 
    uint16_t last = *(uint16_t*)(0xc0019998); // Read last index of memory map
    uint32_t start = 0; 
    print("      Base            Length        Type\n");
    print("---------------- ---------------- --------\n");
    while (start < last) {
        printHex(*(uint32_t*)(0xc0020000 + start + 4)); // High 32 bits of base address
        printHex(*(uint32_t*)(0xc0020000 + start)); // Low 32 bits of base address
        print(" ");
        printHex(*(uint32_t*)(0xc0020000 + start + 12)); // High 32 bits of length 
        printHex(*(uint32_t*)(0xc0020000 + start + 8)); // Low 32 bits of length
        print(" ");
        printHex(*(uint32_t*)(0xc0020000 + start + 16)); // Memory region type
        print("\n");
        
        
        start+=size; // Increment start by size
    }
}