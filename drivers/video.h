#ifndef __VIDEO
#define __VIDEO

#include "../utils/ports.h"

void clear(uint8_t newColor);
void update_cursor();
void scroll();
void print(const char *str);
void printHex(uint32_t no);
void initVideo();
#endif