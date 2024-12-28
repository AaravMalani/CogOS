#ifndef __VIDEO
#define __VIDEO

#include "../utils/ports.h"

void clear(uint8_t newColor); // Clear the screen to a color and reset VGA cursor
void update_cursor();         // Update VGA cursor
void scroll();                // Scroll screen up a line
void print(const char *str);  // Print a null-terminated string
void printHex(uint32_t no);   // Print a number in hexadecimal

#endif