#ifndef __IO_FLOPPY
#define __IO_FLOPPY
#include <utils/types.h>
const char* floppyTypes[] = {
    "no drive",
    "360 KB 5.25 Drive",
    "1.2 MB 5.25 Drive",
    "720 KB 3.5 Drive",
    "1.44 MB 3.5 Drive",
    "2.88 MB 3.5 drive"
};
uint8_t floppyFind();
#endif