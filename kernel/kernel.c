#include <drivers/video.h>
#include <drivers/io/interrupts.h>
#include <drivers/io/floppy.h>
#include <drivers/io/pic.h>
#include <drivers/mm.h>
#include <utils/disk.h>
int main(void)
{
    initPaging(); // Initialise paging 
    clear(0x3b); // Clear the screen
    print("CogOS: A 32 Bit OS!\n"); 
    remapPIC(); // Remap the PIC
    loadIDT(); // Load Interrupt Descriptor Table
    readMemory(); // Print memory map
    uint8_t floppyType = floppyFind();
    print("master floppy = ");
    print(floppyTypes[(floppyType & 0xF0)>>4]);
    print(", slave floppy = ");
    print(floppyTypes[floppyType & 0xF]);
    print("\n");
    struct MBR* mbr = (struct MBR*) 0xc0007c00;
    print("Unique Disk ID: ");
    printHex(mbr->uniqueDiskId);
    print("\n");
    char* s = "Partition 1: ";
    for (int i = 0; i < 4; i++) {
        print(s);
        s[10]++;
        if (!mbr->partTableEntries[i].totalSectors) {
            print("nonexistent\n");
            continue;
        } 
        if (mbr->partTableEntries[i].attributes & 0x80) {
            print("active, ");
        }
        printHex(mbr->partTableEntries[i].relativeSector);
        print(" - ");
        printHex(mbr->partTableEntries[i].relativeSector + mbr->partTableEntries[i].totalSectors - 1);
        print("\n");
    }

    while (1);
}