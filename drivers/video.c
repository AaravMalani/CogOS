#include "video.h"

volatile uint8_t x = 0, y = 0;
volatile uint8_t color = 0;

void clear(uint8_t newColor)
{
    color = newColor; // Set the color to the new color
    uint16_t val = ' ' | (newColor << 8); 
    for (uint32_t i = 0; i < (80 * 48); i++) // Iterates over every coordinate
    {
        *(unsigned short *)(0xc00b8000 + (i * 2)) = val; // Sets the location to a space with the color newColor
    }
    x = 0; // Reset the x and y coordinates
    y = 0;
    update_cursor(); // Update VGA cursor
}
void update_cursor() {
    uint16_t pos = ((uint16_t)y)*80 + (uint16_t)x; // Get the position from X and Y 
    outb(0x3D4, 0x0F); 
    outb(0x3D5, (uint8_t)pos&0xFF); // Pass the lower 8 bits of the position
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(pos>>8)&0xFF); // Pass the higher 8 bits of the position
    
}
void scroll()
{
    for (uint8_t i = 0; i < 24; i++)
    {
        for (uint8_t j = 0; j < 80; j++)
        {
            *(unsigned short *)(0xc00b8000 + (i * 80 * 2) + j * 2) = *(unsigned short *)(0xc00b8000 + ((i + 1) * 80 * 2) + j * 2) | (color << 8); // Move each element of the lower rows to the row above
        }
    }
    for (uint8_t j = 0; j < 80; j++)
    {
        *(unsigned short *)(0xc00b8000 + (24 * 80 * 2) + j * 2) = ' ' | (color << 8); // Clear the lowest row
    }
}

void print(const char *str)
{
    while (*str) // Iterate over string till null byte
    { 
        if (*str == '\n') 
        {
            y++; // Go to next line
            x = 0; // Go to start of line
        }
        else
        {
            *(unsigned short *)(0xc00b8000 + (y * 80 * 2) + (x * 2)) = (*str) | (color << 8); // Set text at position (x, y) to the current value in str
            x++; // Go to next character
        }
        if (x > 79) // Prevent x from becoming greater than 79, 
        {
            y+=x/80;
            x%=80;
        }
        while (y > 24) // If y > 24, we need to scroll
        {
            scroll(); 
            y--;
        }
        str++; // Go to next character in string
    }
    update_cursor(); // Update VGA cursor

    
}

void printHex(uint32_t v) {
    char val[] = "00000000"; // Edit the base string
    for (int i = 7; i >= 0; i--) { // Iterate over string backwards
        if (v%16 > 9) { // Set character to A for values greater than 9
            val[i] = 'A';
            v-=10;
        }
        val[i]+=v%16; // Add the last digit of number to string
        v/=16; // Remove the last digit from number
    }
    print(val); // Print resultant string
}