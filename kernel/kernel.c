#include "../drivers/video.h"
#include "../drivers/io.h"

int main(void)
{
    clear(0x3b);
    print("CogOS: A 32 Bit OS!\n");
    remapPIC();
    loadIDT();
    while (1);
}