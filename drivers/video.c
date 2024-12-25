#include "video.h"
volatile uint8_t x = 0, y = 0;
volatile uint8_t color = 0;

void initVideo() {
    x = 0;
    y = 0;
    color = 0x10;
}
void clear(uint8_t newColor)
{
    color = newColor;
    uint16_t val = ' ' | (newColor << 8);
    for (uint32_t i = 0; i < (80 * 48); i++)
    {
        *(unsigned short *)(0xb8000 + (i * 2)) = val;
    }
    x = 0;
    y = 0;
    update_cursor();
}
void update_cursor() {
    uint16_t pos = ((uint16_t)y)*80 + (uint16_t)x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)pos&0xFF);
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(pos>>8)&0xFF);
    
}
void scroll()
{
    for (uint8_t i = 0; i < 24; i++)
    {
        for (uint8_t j = 0; j < 80; j++)
        {
            *(unsigned short *)(0xb8000 + (i * 80 * 2) + j * 2) = *(unsigned short *)(0xb8000 + ((i + 1) * 80 * 2) + j * 2) | (color << 8);
        }
    }
    for (uint8_t j = 0; j < 80; j++)
    {
        *(unsigned short *)(0xb8000 + (24 * 80 * 2) + j * 2) = ' ' | (color << 8);
    }
}

void print(const char *str)
{
    
    while (*str)
    {
        if (*str == '\n')
        {
            y++;
            x = 0;
        }
        else
        {
            *(unsigned short *)(0xb8000 + (y * 80 * 2) + (x * 2)) = (*str) | (color << 8);
            x++;
        }
        while (x > 79)
        {
            x -= 80;
            y++;
        }
        while (y > 24)
        {
            scroll();
            y--;
        }
        str++;
    }
    update_cursor();

    
}

void printHex(uint32_t v) {
    char val[] = "00000000\n";
    for (int i = 7; i >= 0; i--) {
        if (v%16 > 9) {
            val[i] = 'A';
            v-=10;
        }
        val[i]+=v%16;
        v/=16;
    }
    print(val);
}