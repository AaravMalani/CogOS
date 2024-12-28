#ifndef __MM
#define __MM
void initPaging(); // Generate page directory and tables and enable paging
void readMemory(); // Read memory map loaded in bootloader at 0x20000
#endif